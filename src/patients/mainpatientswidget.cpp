#include "mainpatientswidget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "macros.h"
#include "patients.h"
#include "patienteditwidget.h"

#include "components/decodingproxymodel.h"
#include "components/yosortfilterproxymodel.h"
#include "components/columnjoiningproxymodel.h"


MainPatientsWidget::MainPatientsWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainPatientsWidget::init()
{
	m_queryModel = new QSqlQueryModel(this);

	DecodingProxyModel* proxy = new DecodingProxyModel(this);
	proxy->setInterfacesPtr(Patients::interfaces);
	proxy->addEncodedStringColumn(1);
	proxy->addEncodedStringColumn(2);
	proxy->addEncodedStringColumn(3);
	proxy->addEncodedDateColumn(4);
	proxy->setModel(m_queryModel);

	m_sortModel = new YoSortFilterProxyModel(this);
	m_sortModel->setSourceModel(proxy);
	m_sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterKeyColumn(1);

	updatePatientsList();

	m_view->setModel(m_sortModel);
	m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_view->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
	m_view->setColumnHidden(0, true);

	selectionChanged();
}


void MainPatientsWidget::initConnections()
{
	connect(m_addPatient, SIGNAL(clicked()), SLOT(addPatient()));
	connect(m_editPatient, SIGNAL(clicked()), SLOT(editPatient()));
	connect(m_deletePatient, SIGNAL(clicked()), SLOT(deletePatient()));

	connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(editPatient()));

	connect(m_view->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));

	connect(m_searchWidget, SIGNAL(searchPressed()), SLOT(updatePatientsList()));
}


QString MainPatientsWidget::patientListQuery() const
{
	static const QString query =
			" SELECT id, familyName AS \"Фамилия\", name AS \"Имя\", "
				" patronymic AS \"Отчество\", birthday AS \"Дата рождения\""
			" FROM Patient "
			" ORDER BY familyName, name, patronymic ";

	return query;
}


void MainPatientsWidget::updatePatientsList()
{
	m_queryModel->setQuery(patientListQuery());
	// Загружаем все записи.
	while(m_queryModel->canFetchMore())
		m_queryModel->fetchMore();

	m_sortModel->setFilterWildcard(m_searchWidget->enteredText());
	m_sortModel->sort(1);	// Колонка с фамилией.

	checkQuery(m_queryModel->query());
}


void MainPatientsWidget::addPatient()
{
	const QString& searchText = m_searchWidget->enteredText();

	// То, что пользователь ввел в строку, разделенное пробельными символами.
	// Содержит возможное имя будущего пациента.
	QStringList patientName;
	if(!searchText.isEmpty() && m_sortModel->rowCount() == 0)
		patientName = searchText.split(QRegExp("\\s+"));


	PatientEditWidget* widget = NULL;

	// Не в интервале от 1 до 3 строк.
	if(patientName.count() < 1 || 3 < patientName.count())
		widget = new PatientEditWidget(PatientEditWidget::InvalidId, this);
	else
	{
		// Добавляем в конец элементы до нужного количества (3).
		if(patientName.count() == 1)
			patientName << QString::null << QString::null;
		else if(patientName.count() == 2)
			patientName << QString::null;

		widget = new PatientEditWidget(patientName[0], patientName[1],
									   patientName[2], this);
	}


	if(widget != NULL)
	{
		connect(widget, SIGNAL(saved()), SLOT(updatePatientsList()));
		connect(widget, SIGNAL(saved()), SLOT(selectionChanged()));
		emit requestToAddNewWidget(widget, widget->fullPatientName());
	}
}


void MainPatientsWidget::editPatient()
{
	const int patientId = selectedPatientId();
	Q_ASSERT(patientId > 0);

	PatientEditWidget* widget = new PatientEditWidget(patientId, this);
	connect(widget, SIGNAL(saved()), SLOT(updatePatientsList()));
	emit requestToAddNewWidget(widget, widget->fullPatientName());
}


void MainPatientsWidget::deletePatient()
{
	const int patientId = selectedPatientId();
	Q_ASSERT(patientId > 0);

	const QString& title = "Удаление пациента";
	const QString& descr = "Вы действительно хотите удалить пациента?\n"
						   "Будут также удалены все осмотры данного пациента.";
	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);
	if(rc == QMessageBox::Yes)
	{
		QSqlQuery q;

		q.exec("BEGIN");

		q.prepare("DELETE FROM Address WHERE patientId = :id");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM Document WHERE patientId = :id");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.prepare(" DELETE FROM ExaminationData WHERE examinationId IN "
				  " (SELECT id FROM Examination WHERE patientId = ?) ");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM Examination WHERE patientId = ?");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM Patient WHERE id = :id");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.exec("COMMIT");

		updatePatientsList();
		selectionChanged();
	}
}


void MainPatientsWidget::selectionChanged()
{
	const bool enableAdding =
			!Patients::interfaces->demo->isDemoVersion() ||
			(m_queryModel->rowCount() < Patients::interfaces->demo->patientCountLimit());
	m_addPatient->setEnabled(enableAdding);


	const bool enableEditing = onePatientSelected();
	m_editPatient->setEnabled(enableEditing);
	m_deletePatient->setEnabled(enableEditing);
}


bool MainPatientsWidget::onePatientSelected() const
{
	return 	m_view->selectionModel() != NULL &&
			m_view->selectionModel()->hasSelection() &&
			m_view->selectionModel()->selectedRows().count() == 1;
}


int MainPatientsWidget::selectedPatientId() const
{
	int id = -1;

	if(onePatientSelected())
	{
		const int row = m_sortModel->mapToSource(m_view->currentIndex()).row();
		id = m_queryModel->record(row).value(0).toInt();
	}

	return id;
}
