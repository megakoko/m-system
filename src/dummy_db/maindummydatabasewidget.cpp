#include "maindummydatabasewidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"

#include "dummydatabase.h"
#include "dummypatients.h"
#include "dummydepartments.h"
#include "dummyexaminations.h"


MainDummyDatabaseWidget::MainDummyDatabaseWidget(QWidget *parent)
	: PluginWidget(parent)
	, m_dummyData(new DummyData)
{
	setupUi(this);

	init();
	initConnections();
}


void MainDummyDatabaseWidget::init()
{
	updateInformation();
}


void MainDummyDatabaseWidget::initConnections()
{
	connect(m_createPatients, SIGNAL(clicked()), SLOT(createPatients()));
	connect(m_createStaff, SIGNAL(clicked()), SLOT(createStaff()));
	connect(m_createDepartments, SIGNAL(clicked()), SLOT(createDepartments()));
	connect(m_createExaminations, SIGNAL(clicked()), SLOT(createExaminations()));
}


void MainDummyDatabaseWidget::updateInformation()
{
	// Пациенты.
	QSqlQuery q("SELECT COUNT(*) FROM Patient");
	checkQuery(q);

	q.first();
	const int numberOfPatientsInDatabase = q.value(0).toInt();
	m_patientsCount->setNum(numberOfPatientsInDatabase);

	// Ограничиваем количество создаваемых пациентов.
	if(DummyDatabase::interfaces->demo->isDemoVersion())
	{
		const int max = DummyDatabase::interfaces->demo->patientCountLimit() -
						numberOfPatientsInDatabase;
		m_createPatientsCount->setMaximum(qMax(max, 0));
	}


	// Ограничиваем количество создаваемых осмотров.
	if(DummyDatabase::interfaces->demo->isDemoVersion())
	{
		q.exec("SELECT COUNT(*) FROM Examination");
		checkQuery(q);
		q.first();

		const int numberOfExaminationsInDatabase = q.value(0).toInt();
		const int max = DummyDatabase::interfaces->demo->examinationCountLimit() -
						numberOfExaminationsInDatabase;

		m_createExaminationsCount->setMaximum(qMax(max, 0));
	}
}


void MainDummyDatabaseWidget::createPatients()
{
	const int patientsCount = m_createPatientsCount->value();
	DummyPatients p(m_dummyData);
	p.createPatients(patientsCount);

	updateInformation();
}


void MainDummyDatabaseWidget::createStaff()
{
	DummyDepartments d(m_dummyData);
	d.createStaff(m_createStaffCount->value());

	updateInformation();
}


void MainDummyDatabaseWidget::createDepartments()
{
	DummyDepartments d(m_dummyData);
	d.createDepartments();

	updateInformation();
}


void MainDummyDatabaseWidget::createExaminations()
{
	DummyExaminations e(m_dummyData);
	e.createExaminations(m_createExaminationsCount->value(),
						 m_createExaminationsPercentage->value());

	updateInformation();
}
