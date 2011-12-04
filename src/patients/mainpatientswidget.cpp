#include "mainpatientswidget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>


#include "macros.h"
#include "patienteditwidget.h"


static const QString patientListQuery = QString::fromUtf8(
	" SELECT id, familyName AS Фамилия, name AS Имя, patronymic AS Отчество "
	" FROM Patient "
	" ORDER BY familyName, name, patronymic ");

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
	m_model->setQuery(patientListQuery);


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
}


void MainPatientsWidget::updatePatientsList()
{
	m_model->setQuery(patientListQuery);
}


void MainPatientsWidget::addPatient()
{
	const QString& basename = QString::fromUtf8("Фамилия");

	/*
		Regexp explanation 'basename \\((\\d+)\\)$':
		basename => ( => \d+ => ) => $.
	*/
	QSqlQuery q;
	q.prepare(QString(" SELECT SUBSTRING(familyName, E'%1 \\\\((\\\\d+)\\\\)$') AS Number "
					  " FROM Patient ORDER BY Number").arg(basename));
	q.exec();
	checkQuery(q);


	QList<int> numbers;
	while(q.next())
	{
		if(q.isNull(0))
			continue;
		numbers << q.value(0).toInt();
	}

	int possibleNumber = 0;
	while(++possibleNumber > 0)
		if(!numbers.contains(possibleNumber))
			break;

	Q_ASSERT(possibleNumber > 0);
	QString("%1 (%2)").arg(basename).arg(possibleNumber);

	q.prepare(" INSERT INTO Patient (name, familyname, patronymic) "
			  " VALUES(:name, :familyname, :patronymic)");
	q.bindValue(":name", QString("%1 (%2)").arg(QString::fromUtf8("Имя"))
										   .arg(possibleNumber));
	q.bindValue(":familyname", QString("%1 (%2)").arg(basename).arg(possibleNumber));
	q.bindValue(":patronymic", QString("%1 (%2)").arg(QString::fromUtf8("Отчество"))
												 .arg(possibleNumber));
	q.exec();
	checkQuery(q);



	updatePatientsList();
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
