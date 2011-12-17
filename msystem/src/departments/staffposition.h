#pragma once

#include <QString>

class StaffPositionEditDialog;
class QSqlRecord;

class StaffPosition
{
	friend class StaffPositionEditDialog;
public:
	StaffPosition(const int departmentId);
	StaffPosition(const QSqlRecord& rec);

	void markForDeletion();

	void save() const;

	QString toString() const;

private:
	int m_id;

	int m_departmentId;
	int m_staffId;
	int m_positionId;

	bool m_markedForDeletion;
};
