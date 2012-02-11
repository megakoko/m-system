#include "departmenteditwidget.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

#include "departments.h"
#include "macros.h"
#include "staffpositioneditdialog.h"


DepartmentEditWidget::DepartmentEditWidget(const int departmentId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_departmentId(departmentId)
{
    setupUi(this);

	init();
}


QString DepartmentEditWidget::departmentName() const
{
	return QString::fromUtf8("Отделение ") + m_name->text();
}


void DepartmentEditWidget::init()
{
	m_name->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("Department", "name"));
	
	m_shortName->setMaxLength(Departments::interfaces->db->
							  fieldMaximumLength("Department", "shortName"));


	connect(m_name, SIGNAL(editingFinished()), SLOT(nameChanged()));
	connect(m_save, SIGNAL(clicked()), SIGNAL(requestToCloseTab()));

	connect(m_departmentStaff, SIGNAL(doubleClicked(QModelIndex)),
			SLOT(editStaffPosition()));

	connect(m_departmentStaff, SIGNAL(itemSelectionChanged()),
			SLOT(staffPositionSelectionChanged()));

	connect(m_addStaffToDepartment, SIGNAL(clicked()), SLOT(addStaffPosition()));
	connect(m_editStaffInDepartment, SIGNAL(clicked()), SLOT(editStaffPosition()));
	connect(m_deleteStaffFromDepartment, SIGNAL(clicked()), SLOT(deleteStaffPosition()));


	initComboboxes();

	if(m_departmentId != InvalidId)
	{
		QSqlQuery q;

		q.prepare(" SELECT name, shortName, typeId, headOfDepartmentId "
				  " FROM Department "
				  " WHERE id = :id ");
		q.addBindValue(m_departmentId);
		q.exec();
		checkQuery(q);

		const bool idIsValid = q.first();
		Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

		m_name->setText(q.value(0).toString());
		m_shortName->setText(q.value(1).toString());
		m_type->setCurrentIndex(m_type->findData(q.value(2).toInt()));
		m_headOfDepartment->setCurrentIndex(
					m_headOfDepartment->findData(q.value(3).toInt()));


		q.prepare(" SELECT id, departmentId, staffId, positionId "
				  " FROM DepartmentStaffPosition "
				  " WHERE departmentId = ? ");
		q.addBindValue(m_departmentId);
		q.exec();
		checkQuery(q);

		while(q.next())
		{
			m_staffPosition << StaffPosition(q.record());
			m_departmentStaff->addItem(m_staffPosition.last().toString());
		}


	}
	staffPositionSelectionChanged();
}


void DepartmentEditWidget::initComboboxes()
{
	QSqlQuery q;
	q.exec(" SELECT id, familyName || ' ' || name || ' ' || patronymic AS Имя "
		   " FROM Staff");
	checkQuery(q);
	while(q.next())
		m_headOfDepartment->addItem(q.value(1).toString(), q.value(0));
	m_headOfDepartment->setCurrentIndex(-1);


	q.exec("SELECT id, name FROM DepartmentType");
	checkQuery(q);
	while(q.next())
		m_type->addItem(q.value(1).toString(), q.value(0));
	m_type->setCurrentIndex(-1);
}


bool DepartmentEditWidget::canSave(QString &errorDescription) const
{
	if (m_name->text().simplified().isEmpty() ||
		m_shortName->text().simplified().isEmpty())
	{
		errorDescription = QString::fromUtf8("Имя отделения не заполнено");
		return false;
	}
	else if(m_type->currentIndex() < 0)
	{
		errorDescription = QString::fromUtf8("Не выбран тип отделения");
		return false;
	}
	else if(m_headOfDepartment->currentIndex() < 0)
	{
		errorDescription = QString::fromUtf8("Не выбран глава отделения");
		return false;
	}

	return true;
}


void DepartmentEditWidget::save()
{
	QSqlQuery q;
	if(m_departmentId == InvalidId)
	{
		q.prepare(" INSERT INTO Department "
				  " ( name,  shortName,  typeid,  headOfDepartmentId) VALUES "
				  " (:name, :shortName, :typeid, :headOfDepartmentId) " +
				  Departments::interfaces->db->returningSentence("id"));
	}
	else
	{
		q.prepare(" UPDATE Department SET "
				  " name = :name, "
				  " shortName = :shortName, "
				  " typeid = :typeid, "
				  " headOfDepartmentId = :headId "
				  " WHERE id = :id ");
		q.bindValue(":id", m_departmentId);
	}
	q.bindValue(":name", nullIfEmpty(m_name->text().simplified()));
	q.bindValue(":shortName", nullIfEmpty(m_shortName->text().simplified()));
	q.bindValue(":typeid", m_type->itemData(m_type->currentIndex()));
	q.bindValue(":headId",
				m_headOfDepartment->itemData(m_headOfDepartment->currentIndex()));
	q.exec();
	checkQuery(q);


	if(m_departmentId == InvalidId)
		m_departmentId = Departments::interfaces->db->lastInsertedId(&q).toInt();

	Q_ASSERT(m_departmentId != InvalidId);


	foreach(const StaffPosition& sp, m_staffPosition)
		sp.save(m_departmentId);
	foreach(const StaffPosition& sp, m_staffPositionMarkedForDeletion)
		sp.save(m_departmentId);

	emit saved();
}


void DepartmentEditWidget::nameChanged()
{
	emit requestToSetTabLabel(departmentName());
}


void DepartmentEditWidget::staffPositionSelectionChanged()
{
	const bool disableButtons = m_departmentStaff->selectedItems().size() != 1;

	m_editStaffInDepartment->setDisabled(disableButtons);
	m_deleteStaffFromDepartment->setDisabled(disableButtons);
}


void DepartmentEditWidget::addStaffPosition()
{
	StaffPositionEditDialog d(StaffPosition(), this);
	if(d.exec() == QDialog::Accepted)
	{
		m_staffPosition << d.currentStaffPosition();
		m_departmentStaff->addItem(m_staffPosition.last().toString());
	}
}


void DepartmentEditWidget::editStaffPosition()
{
	const int index = m_departmentStaff->currentRow();


	StaffPositionEditDialog d(m_staffPosition.at(index), this);
	if(d.exec() == QDialog::Accepted)
	{
		m_staffPosition[index] = d.currentStaffPosition();
		m_departmentStaff->item(index)->setText(m_staffPosition.at(index).toString());
	}
}


void DepartmentEditWidget::deleteStaffPosition()
{
	const QString& title = "Удаление сотрудника из отделения";
	const QString& descr = "Вы действительно хотите удалить сотрудника из отделения?";

	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No,
										 QMessageBox::No);
	if(rc == QMessageBox::Yes)
	{
		const int index = m_departmentStaff->currentRow();
		m_staffPositionMarkedForDeletion << m_staffPosition.takeAt(index);
		m_staffPositionMarkedForDeletion.last().markForDeletion();

		delete m_departmentStaff->takeItem(index);
	}
}
