#include "mainpatientswidget.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>

#include <QDebug>


#include "../loader/macros.h"


static const QString patientListQuery = QString::fromUtf8(
	" SELECT id, familyName AS Фамилия, "
	" name AS Имя, patronymic AS Отчество FROM Patient ORDER BY id");

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
}


void MainPatientsWidget::initConnections()
{
	connect(m_addPatient, SIGNAL(clicked()), SLOT(addPatient()));
	connect(m_editPatient, SIGNAL(clicked()), SLOT(editPatient()));
	connect(m_deletePatient, SIGNAL(clicked()), SLOT(deletePatient()));


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
		if(q.isNull(0))	// Although null values will appear last, we can use 'break' here.
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

}


void MainPatientsWidget::deletePatient()
{

}
