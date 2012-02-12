#include "maindummydatabasewidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"

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
	connect(m_updateInformation, SIGNAL(clicked()), SLOT(updateInformation()));
	connect(m_createPatients, SIGNAL(clicked()), SLOT(createPatients()));
	connect(m_createStaff, SIGNAL(clicked()), SLOT(createStaff()));
	connect(m_createDepartments, SIGNAL(clicked()), SLOT(createDepartments()));
	connect(m_createExaminations, SIGNAL(clicked()), SLOT(createExaminations()));
}


void MainDummyDatabaseWidget::updateInformation()
{
	QSqlQuery q("SELECT COUNT(*) FROM Patient");
	checkQuery(q);

	q.first();
	m_patientsCount->setNum(q.value(0).toInt());

	q.exec("SELECT COUNT(*) FROM Staff");
	checkQuery(q);
	q.first();
	m_staffCount->setNum(q.value(0).toInt());
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
