#include "maindepartmentswidget.h"



#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

#include "macros.h"
#include "healthfacilityeditdialog.h"
#include "positionseditdialog.h"
#include "staffeditwidget.h"
#include "departmenteditwidget.h"


const QString staffQuery = QString::fromUtf8(
	" SELECT id, familyName || ' ' || "
		" substring(name, 1, 1) || '. ' || "
		" substring(patronymic, 1, 1) || '.' || "
		" COALESCE (' (' || specialization || ')', '') "
	" FROM Staff"
	" ORDER BY id");

const QString departmentQuery = QString::fromUtf8(
	" SELECT id, name AS \"Имя отделения\""
	" FROM Department "
	" ORDER BY id");


MainDepartmentsWidget::MainDepartmentsWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainDepartmentsWidget::init()
{
	m_healthFacilityInformation->setText(HealthFacilityEditDialog::shortInformation());

	m_positionsInformation->setText(PositionsEditDialog::positions().join(", "));


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

	connect(m_editPositions, SIGNAL(clicked()), SLOT(editPositions()));

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
		m_healthFacilityInformation->setText(HealthFacilityEditDialog::shortInformation());
	}
}


void MainDepartmentsWidget::editPositions()
{
	PositionsEditDialog d(this);
	if(d.exec() == QDialog::Accepted)
		m_positionsInformation->setText(PositionsEditDialog::positions().join(", "));
}


void MainDepartmentsWidget::updateStaffList()
{
	m_staffModel->setQuery(staffQuery);
}


void MainDepartmentsWidget::addStaff()
{
	StaffEditWidget* w = new StaffEditWidget(StaffEditWidget::InvalidId, this);
	addNewWidget(w, w->staffName());
	connect(w, SIGNAL(saved()), SLOT(updateStaffList()));
}


void MainDepartmentsWidget::editStaff()
{
	StaffEditWidget* w = new StaffEditWidget(selectedStaffId(), this);
	addNewWidget(w, w->staffName());
	connect(w, SIGNAL(saved()), SLOT(updateStaffList()));
}


void MainDepartmentsWidget::deleteStaff()
{
	const int id = selectedStaffId();
	Q_ASSERT(id > 0);

	if(staffIsHeadOfDepartment(id))
	{
		QSqlQuery q;
		q.prepare(" SELECT name FROM Department WHERE headOfDepartmentId = ? LIMIT 5");
		q.addBindValue(id);
		q.exec();
		checkQuery(q);

		QStringList departments;
		while(q.next())
			departments << q.value(0).toString();

		const QString& title = "Удаление работника";
		const QString& descr =
					QString("Невозможно удалить работника, т.к. он является главной ") +
					(departments.size() > 1 ? "отделений" : "отделения")  + ": " +
					departments.join(", ") + ".";

		QMessageBox::information(this, title, descr);
	}
	else
	{
		const QString& title = "Удаление работника";
		const QString& descr = "Вы действительно хотите удалить работника? Если "
							   "Будет удалена вся информация о нем.";

		const int rc = QMessageBox::question(this, title, descr,
											 QMessageBox::Ok | QMessageBox::Cancel,
											 QMessageBox::Cancel);

		if(rc == QMessageBox::Ok)
		{
			QSqlQuery q;
			q.prepare(" DELETE FROM DepartmentStaffPosition "
					  " WHERE staffId = ? ");
			q.addBindValue(id);
			q.exec();
			checkQuery(q);


			q.prepare(" DELETE FROM Staff "
					  " WHERE id = ? ");
			q.addBindValue(id);
			q.exec();
			checkQuery(q);

			updateStaffList();
		}
	}
}


bool MainDepartmentsWidget::staffIsHeadOfDepartment(const int staffId) const
{
	Q_ASSERT(staffId > 0);
	QSqlQuery q;
	q.prepare("SELECT COUNT(*) FROM Department WHERE headOfDepartmentId = ?");
	q.addBindValue(staffId);
	q.exec();
	checkQuery(q);

	q.first();
	return q.value(0).toInt();
}


void MainDepartmentsWidget::staffSelectionChanged()
{
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
	DepartmentEditWidget* w = new DepartmentEditWidget(DepartmentEditWidget::InvalidId,
													   this);
	addNewWidget(w, w->departmentName());
	connect(w, SIGNAL(saved()), SLOT(updateDepartmentList()));
}


void MainDepartmentsWidget::editDepartment()
{
	DepartmentEditWidget* w = new DepartmentEditWidget(selectedDepartmentId(), this);
	addNewWidget(w, w->departmentName());
	connect(w, SIGNAL(saved()), SLOT(updateDepartmentList()));
}


void MainDepartmentsWidget::deleteDepartment()
{
	const QString& title = "Удаление отделения";
	const QString& descr = "Вы действительно хотите удалить отделение?";
	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Ok | QMessageBox::Cancel,
										 QMessageBox::Cancel);


	if(rc == QMessageBox::Ok)
	{
		const int id = selectedDepartmentId();
		Q_ASSERT(id > 0);


		QSqlQuery q;
		q.prepare("DELETE FROM DepartmentStaffPosition WHERE departmentId = ?");
		q.addBindValue(id);
		q.exec();
		checkQuery(q);


		q.prepare("DELETE FROM Department WHERE id = ?");
		q.addBindValue(id);
		q.exec();
		checkQuery(q);


		updateDepartmentList();
	}
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


