#include "mainpatientswidget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>

#include "macros.h"
#include "patients.h"
#include "patienteditwidget.h"


MainPatientsWidget::MainPatientsWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainPatientsWidget::init()
{
	m_model = new QSqlQueryModel(this);
	m_model->setQuery(patientListQuery());


	m_view->setModel(m_model);
	m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_view->setColumnHidden(0, true);

	m_editPatient->setEnabled(false);
	m_deletePatient->setEnabled(false);
}


void MainPatientsWidget::initConnections()
{
	connect(m_addPatient, SIGNAL(clicked()), SLOT(addPatient()));
	connect(m_editPatient, SIGNAL(clicked()), SLOT(editPatient()));
	connect(m_deletePatient, SIGNAL(clicked()), SLOT(deletePatient()));

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
	static const QString select =	" SELECT id, familyName AS Фамилия, "
									" name AS Имя, patronymic AS Отчество ";
	static const QString from =		" FROM Patient ";
	static const QString where =	" WHERE familyName ILIKE '%%1%' ";
	static const QString orderby =	" ORDER BY familyName, name, patronymic ";


	const QString& filter = m_searchline->text().simplified().remove('\'');


	return filter.isEmpty() ? (select + from + orderby)
							: (select + from + where.arg(filter) + orderby);
}


void MainPatientsWidget::updatePatientsList()
{
	m_model->setQuery(patientListQuery());
}


void MainPatientsWidget::addPatient()
{
	const QString& searchText = m_searchline->text();

	// То, что пользователь ввел в строку, разделенное пробельными символами.
	// Содержит возможное имя будущего пациента.
	QStringList patientName;
	if(!searchText.isEmpty() && m_model->rowCount() == 0)
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
		addNewWidget(widget, widget->fullPatientName());
	}
}


void MainPatientsWidget::editPatient()
{
	const int patientId = selectedPatientId();
	Q_ASSERT(patientId > 0);

	PatientEditWidget* widget = new PatientEditWidget(patientId, this);
	connect(widget, SIGNAL(saved()), SLOT(updatePatientsList()));
	addNewWidget(widget, widget->fullPatientName());
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
		id = m_model->record(m_view->currentIndex().row()).value(0).toInt();

	return id;
}
