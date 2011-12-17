#pragma once

#include <QString>

class QSqlRecord;

class Address
{
public:
	Address(bool isMailingAddress);
	Address(const QSqlRecord& record);

	QString toString() const;
	bool isNull() const;

	void save(const int patientId) const;
	void deleteAddress() const;

	QString city;
	QString street;
	QString house;
	QString apartment;

private:

	bool m_isMailingAddress;
	mutable int m_id;

};

