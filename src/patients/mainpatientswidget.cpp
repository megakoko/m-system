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
	proxy->addColumnToDecode(1);
	proxy->addColumnToDecode(2);
	proxy->addColumnToDecode(3);
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

	m_editPatient->setEnabled(false);
	m_deletePatient->setEnabled(false);
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

	connect(m_searchline, SIGNAL(returnPressed()), SLOT(updatePatientsList()));
	connect(m_find, SIGNAL(clicked()), SLOT(updatePatientsList()));
	connect(m_clear, SIGNAL(clicked()), m_searchline, SLOT(clear()));
	connect(m_clear, SIGNAL(clicked()), SLOT(updatePatientsList()));
}


QString MainPatientsWidget::patientListQuery() const
{
	static const QString select=" SELECT id, familyName AS \"Фамилия\", name AS \"Имя\", "
								" patronymic AS \"Отчество\", %1 AS \"Дата рождения\"";
	static const QString psqlSelect = select.arg("to_char(birthday, 'dd.mm.yyyy')");
	static const QString sqliteSelect = select.arg("strftime('%d.%m.%Y', birthday)");
	static const QString from  =" FROM Patient ";
	static const QString order =" ORDER BY familyName, name, patronymic ";


	QString query;

	switch(Patients::interfaces->db->currentSqlDriver())
	{
	case DatabaseInterface::PSQL:
		query = psqlSelect;
		break;
	case DatabaseInterface::SQLITE:
		query = sqliteSelect;
		break;
	default:
		qFatal("Unknown sql driver");
	}

	query += from + order;

	return query;
}


void MainPatientsWidget::updatePatientsList()
{
	m_queryModel->setQuery(patientListQuery());
	// Загружаем все записи.
	while(m_queryModel->canFetchMore())
		m_queryModel->fetchMore();

	m_sortModel->setFilterWildcard(m_searchline->text());
	m_sortModel->sort(1);	// Колонка с фамилией.

	checkQuery(m_queryModel->query());
}


void MainPatientsWidget::addPatient()
{
	const QString& searchText = m_searchline->text();

	// То, что пользователь ввел в строку, разделенное пробельными символами.
	// Содержит возможное имя будущего пациента.
	QStringList patientName;
	if(!searchText.isEmpty() && m_queryModel->rowCount() == 0)
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

	const QString& title = QString::fromUtf8("Удаление пациента");
	const QString& descr = QString::fromUtf8("Вы действительно хотите удалить пациента?");
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

		q.prepare("DELETE FROM Patient WHERE id = :id");
		q.addBindValue(patientId);
		q.exec();
		checkQuery(q);

		q.exec("COMMIT");

		updatePatientsList();
	}
}


void MainPatientsWidget::selectionChanged()
{
	const bool enableButtons = onePatientSelected();
	m_editPatient->setEnabled(enableButtons);
	m_deletePatient->setEnabled(enableButtons);
}


bool MainPatientsWidget::onePatientSelected() const
{
	return 	m_view->selectionModel()->hasSelection() &&
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
