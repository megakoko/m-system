#include "departmeneditwidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"


DepartmenEditWidget::DepartmenEditWidget(const int departmentId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_departmentId(departmentId)
{
    setupUi(this);

	init();
}


void DepartmenEditWidget::init()
{
	connect(m_save, SIGNAL(clicked()), SIGNAL(closeMe()));

	Q_ASSERT(m_departmentId > 0);

	QSqlQuery q;
	q.exec(" SELECT id, familyName || ' ' || substring(name, 1, 1) || '. ' "
		   " || substring(patronymic, 1, 1) || '.' AS Имя "
		   " FROM Staff");
	checkQuery(q);
	while(q.next())
		m_headOfDepartment->addItem(q.value(1).toString(), q.value(0));


	q.exec("SELECT id, name FROM DepartmentType");
	checkQuery(q);
	while(q.next())
		m_type->addItem(q.value(1).toString(), q.value(0));


	q.prepare(" SELECT name, shortName, typeId, headOfDepartmentId "
			  " FROM Department "
			  " WHERE id = :id ");
	q.addBindValue(m_departmentId);
	q.exec();
	checkQuery(q);

	const bool idIsValid = q.first();
	Q_ASSERT(idIsValid);

	m_name->setText(q.value(0).toString());
	m_shortName->setText(q.value(1).toString());
	m_type->setCurrentIndex(m_type->findData(q.value(2).toInt()));
	m_headOfDepartment->setCurrentIndex(
				m_headOfDepartment->findData(q.value(3).toInt()));
}


bool DepartmenEditWidget::canSave(QString &errorDescription) const
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


void DepartmenEditWidget::save()
{
	QSqlQuery q;
	q.prepare(" UPDATE Department SET "
			  " name = :name, "
			  " shortName = :shortName, "
			  " typeid = :typeid, "
			  " headOfDepartmentId = :headId "
			  " WHERE id = :id ");
	q.bindValue(":name", nullIfEmpty(m_name->text().simplified()));
	q.bindValue(":shortName", nullIfEmpty(m_shortName->text().simplified()));
	q.bindValue(":typeid", m_type->itemData(m_type->currentIndex()));
	q.bindValue(":headId",
				m_headOfDepartment->itemData(m_headOfDepartment->currentIndex()));
	q.bindValue(":id", m_departmentId);
	q.exec();
	checkQuery(q);

	emit saved();
}
