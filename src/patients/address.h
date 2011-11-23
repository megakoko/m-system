#pragma once

#include <QString>

class QSqlRecord;

class Address
{
public:
	Address(bool isMailingAddress);
	Address(const QSqlRecord& record);

	QString toString() const;

	void save(const int patientId) const;

	QString city;
	QString street;
	QString house;
	QString apartment;

private:
	bool dataIsNull() const;

	bool m_isMailingAddress;
	mutable int m_id;

};

