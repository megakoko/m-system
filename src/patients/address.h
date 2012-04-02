#pragma once

#include <QString>

class QSqlRecord;

class Address
{
public:
	Address(const QString& type);
	Address(const QSqlRecord& record);

	QString toString() const;
	bool isNull() const;

	void save(const int patientId) const;
	void deleteAddress() const;

	QString region;
	QString city;
	QString street;
	QString house;
	QString apartment;

private:

	QString m_adressTypeTextid;
	mutable int m_id;

};

