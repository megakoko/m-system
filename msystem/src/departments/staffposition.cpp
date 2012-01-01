#include "staffposition.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDebug>


#include "macros.h"


StaffPosition::StaffPosition(const int departmentId = -1)
	: m_id(-1)
	, m_departmentId(departmentId)
	, m_staffId(-1)
	, m_positionId(-1)
	, m_markedForDeletion(false)
{
}


StaffPosition::StaffPosition(const QSqlRecord &rec)
	: m_id(rec.value("id").toInt())
	, m_departmentId(rec.value("departmentId").toInt())
	, m_staffId(rec.value("staffId").toInt())
	, m_positionId(rec.value("positionId").toInt())
	, m_markedForDeletion(false)
{
}


void StaffPosition::markForDeletion()
{
	m_markedForDeletion = true;
}


QString StaffPosition::toString() const
{
	QString result;

	if(m_staffId > 0 && m_positionId > 0)
	{
		QSqlQuery q;
		q.prepare("SELECT name FROM Position WHERE id = ?");
		q.addBindValue(m_positionId);
		q.exec();
		checkQuery(q);

		if(q.first())
			result = q.value(0).toString();
		else
			qWarning() << "Invalid ID in " << __FUNCTION__ << __LINE__;


		q.prepare("SELECT familyName FROM Staff WHERE id = ?");
		q.addBindValue(m_staffId);
		q.exec();
		checkQuery(q);

		if(q.first())
			result += " - " + q.value(0).toString();
		else
			qWarning() << "Invalid ID in " << __FUNCTION__ << __LINE__;
	}

	return result;
}


void StaffPosition::save() const
{
	QSqlQuery q;

	if(m_markedForDeletion)
	{
		if(m_id > 0)
		{
			q.prepare("DELETE FROM DepartmentStaffPosition WHERE id = ?");
			q.addBindValue(m_id);
			q.exec();
			checkQuery(q);
		}
	}
	else
	{
		if(m_id > 0)
		{
			q.prepare(" UPDATE DepartmentStaffPosition SET "
					  " departmentId = :departmentId, "
					  " staffId = :staffId, "
					  " positionId = :positionId "
					  " WHERE id = :id ");
			q.bindValue(":id", m_id);
		}
		else
		{
			q.prepare(" INSERT INTO DepartmentStaffPosition "
					  " (departmentId, staffId, positionId) "
					  " VALUES(:departmentId, :staffId, :positionId)");
		}

		q.bindValue(":departmentId", m_departmentId);
		q.bindValue(":staffId", m_staffId);
		q.bindValue(":positionId", m_positionId);

		q.exec();
		checkQuery(q);
	}
}
