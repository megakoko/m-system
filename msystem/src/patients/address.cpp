#include "address.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include "macros.h"



Address::Address(bool isMailingAddress)
	: m_isMailingAddress(isMailingAddress)
	, m_id(-1)
{

}


Address::Address(const QSqlRecord &rec)
	: city(rec.value("city").toString())
	, street(rec.value("street").toString())
	, house(rec.value("house").toString())
	, apartment(rec.value("apartment").toString())
	, m_isMailingAddress(rec.value("isMailingAddress").toBool())
	, m_id(rec.value("id").toInt())
{

}


QString Address::toString() const
{
	QStringList result;


	if(!city.isNull())
		result << city;

	if(!street.isNull())
		result << street;

	if(!house.isNull())
		result << house;

	if(!apartment.isNull())
		result << apartment;

	return (m_isMailingAddress ? "Адрес прописки: " : "Адрес проживания: ") +
			result.join(", ");
}


void Address::save(const int patientId) const
{
	QSqlQuery q;

	if(isNull())
	{
		deleteAddress();
	}
	else
	{
		if(m_id > 0)
		{
			q.prepare(" UPDATE Address SET "
					  " city = :city, "
					  " street = :street, "
					  " house = :house, "
					  " apartment = :apartment "
					  " WHERE id = :id ");
			q.bindValue(":id", m_id);
		}
		else
		{
			q.prepare(" INSERT INTO Address "
					  " (patientId,  isMailingAddress,  city,  street,  house,  apartment) "
				"VALUES(:patientId, :isMailingAddress, :city, :street, :house, :apartment) "
					  " RETURNING id ");
			q.bindValue(":patientId", patientId);
			q.bindValue(":isMailingAddress", m_isMailingAddress);
		}

		q.bindValue(":city", nullIfEmpty(city));
		q.bindValue(":street", nullIfEmpty(street));
		q.bindValue(":house", nullIfEmpty(house));
		q.bindValue(":apartment", nullIfEmpty(apartment));

		q.exec();
		checkQuery(q);


		// обновляем ID.
		if(m_id <= 0)
		{
			q.first();
			m_id = q.value(0).toInt();
		}
	}
}


void Address::deleteAddress() const
{
	if(m_id > 0)
	{
		QSqlQuery q;
		q.prepare("DELETE FROM Address WHERE id = :id");
		q.bindValue(":id", m_id);

		q.exec();
		checkQuery(q);
	}
}


bool Address::isNull() const
{
	return city.isNull() && street.isNull() && house.isNull() && apartment.isNull();
}
