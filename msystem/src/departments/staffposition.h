#pragma once

#include <QString>

class StaffPositionEditDialog;
class QSqlRecord;

class StaffPosition
{
	friend class StaffPositionEditDialog;
public:
	StaffPosition();
	StaffPosition(const QSqlRecord& rec);

	void markForDeletion();

	void save(const int departmentId) const;

	QString toString() const;

private:
	int m_id;

	int m_staffId;
	int m_positionId;

	bool m_markedForDeletion;
};
