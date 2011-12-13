#include "maindepartmentswidget.h"



#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "macros.h"
#include "healthfacilityeditdialog.h"
#include "staffeditwidget.h"
#include "departmeneditwidget.h"


const QString staffQuery = QString::fromUtf8(
	" SELECT id, familyName || ' ' || substring(name, 1, 1) || '. ' "
	" || substring(patronymic, 1, 1) || '.' AS Имя, specialization AS Специальность "
	" FROM Staff");

const QString departmentQuery = QString::fromUtf8(
	" SELECT id, name AS \"Имя отделения\""
	" FROM Department ");


MainDepartmentsWidget::MainDepartmentsWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainDepartmentsWidget::init()
{
	m_healthFacilityInformation->setText(HealthFacilityEditDialog::toString());


	m_staffModel = new QSqlQueryModel(this);
	m_staffModel->setQuery(staffQuery);

	m_staffTable->setModel(m_staffModel);
	m_staffTable->setColumnHidden(0, true);
	m_staffTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	staffSelectionChanged();


	m_departmentModel = new QSqlQueryModel(this);
	m_departmentModel->setQuery(departmentQuery);

	m_departmentTable->setModel(m_departmentModel);
	m_departmentTable->setColumnHidden(0, true);
	m_departmentTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	departmentSelectionChanged();
}


void MainDepartmentsWidget::initConnections()
{
	connect(m_editHealthFacility, SIGNAL(clicked()), SLOT(editHealthFacility()));

	connect(m_staffTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(staffSelectionChanged()));
	connect(m_addStaff, SIGNAL(clicked()), SLOT(addStaff()));
	connect(m_editStaff, SIGNAL(clicked()), SLOT(editStaff()));
	connect(m_deleteStaff, SIGNAL(clicked()), SLOT(deleteStaff()));


	connect(m_departmentTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(departmentSelectionChanged()));
	connect(m_addDepartment, SIGNAL(clicked()), SLOT(addDepartment()));
	connect(m_editDepartmnet, SIGNAL(clicked()), SLOT(editDepartment()));
	connect(m_deleteDepartment, SIGNAL(clicked()), SLOT(deleteDepartment()));
}


void MainDepartmentsWidget::editHealthFacility()
{
	HealthFacilityEditDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_healthFacilityInformation->setText(HealthFacilityEditDialog::toString());
	}
}


void MainDepartmentsWidget::updateStaffList()
{
	m_staffModel->setQuery(staffQuery);
}


void MainDepartmentsWidget::addStaff()
{
	const QString& basename = QString::fromUtf8("Фамилия");

	QSqlQuery q;
	q.prepare(QString(" SELECT SUBSTRING(familyName, E'%1 \\\\((\\\\d+)\\\\)$') AS Number "
					  " FROM Staff ORDER BY Number").arg(basename));
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

	q.prepare(" INSERT INTO Staff (name, familyname, patronymic) "
			  " VALUES(:name, :familyname, :patronymic)");
	q.bindValue(":name", QString("%1 (%2)").arg(QString::fromUtf8("Имя"))
										   .arg(possibleNumber));
	q.bindValue(":familyname", QString("%1 (%2)").arg(basename).arg(possibleNumber));
	q.bindValue(":patronymic", QString("%1 (%2)").arg(QString::fromUtf8("Отчество"))
												 .arg(possibleNumber));
	q.exec();
	checkQuery(q);



	updateStaffList();
}


void MainDepartmentsWidget::editStaff()
{
	StaffEditWidget* w = new StaffEditWidget(selectedStaffId(), this);
	addNewWidget(w, w->staffName());
	connect(w, SIGNAL(saved()), SLOT(updateStaffList()));
}


void MainDepartmentsWidget::deleteStaff()
{

}


void MainDepartmentsWidget::staffSelectionChanged()
{
	qDebug() << __FUNCTION__;
	const bool selectionCount = selectedStaffCount();
	m_editStaff->setEnabled(selectionCount == 1);
	m_deleteStaff->setEnabled(selectionCount == 1);
}


bool MainDepartmentsWidget::selectedStaffCount() const
{
	return 	m_staffTable->selectionModel()->hasSelection() &&
			m_staffTable->selectionModel()->selectedRows().count() == 1;
}


int MainDepartmentsWidget::selectedStaffId() const
{
	int id = -1;

	if(selectedStaffCount() == 1)
		id = m_staffModel->record(m_staffTable->currentIndex().row()).value(0).toInt();

	return id;
}




void MainDepartmentsWidget::updateDepartmentList()
{
	m_departmentModel->setQuery(departmentQuery);
}


void MainDepartmentsWidget::addDepartment()
{
	const QString& basename = QString::fromUtf8("Отделение");

	QSqlQuery q;
	q.prepare(QString(" SELECT SUBSTRING(name, E'%1 \\\\((\\\\d+)\\\\)$') AS Number "
					  " FROM Department ORDER BY Number").arg(basename));
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

	q.prepare(" INSERT INTO Department (name) "
			  " VALUES(:name)");
	q.bindValue(":name", QString("%1 (%2)").arg(basename).arg(possibleNumber));
	q.exec();
	checkQuery(q);


	updateDepartmentList();
}


void MainDepartmentsWidget::editDepartment()
{
	DepartmenEditWidget* w = new DepartmenEditWidget(selectedDepartmentId(), this);
	addNewWidget(w, w->departmentName());
	connect(w, SIGNAL(saved()), SLOT(updateDepartmentList()));
}


void MainDepartmentsWidget::deleteDepartment()
{

}


void MainDepartmentsWidget::departmentSelectionChanged()
{
	const bool selectionCount = selectedDepartmentCount();
	m_editDepartmnet->setEnabled(selectionCount == 1);
	m_deleteDepartment->setEnabled(selectionCount == 1);
}


bool MainDepartmentsWidget::selectedDepartmentCount() const
{
	return 	m_departmentTable->selectionModel()->hasSelection() &&
			m_departmentTable->selectionModel()->selectedRows().count() == 1;
}


int MainDepartmentsWidget::selectedDepartmentId() const
{
	int id = -1;

	if(selectedDepartmentCount() == 1)
		id = m_departmentModel->record(m_departmentTable->currentIndex().row()).value(0).toInt();

	return id;
}


