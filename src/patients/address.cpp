#include "address.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QStringList>
#include <QDebug>


#include "patients.h"
#include "macros.h"



Address::Address(const QString& type)
	: m_adressTypeTextid(type)
	, m_id(-1)
{

}


Address::Address(const QSqlRecord &rec)
	: city(Patients::interfaces->enc->decode(rec.value("city").toString()))
	, street(Patients::interfaces->enc->decode(rec.value("street").toString()))
	, house(Patients::interfaces->enc->decode(rec.value("house").toString()))
	, apartment(Patients::interfaces->enc->decode(rec.value("apartment").toString()))
	, m_adressTypeTextid(rec.value("textid").toBool())
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

	return (m_adressTypeTextid == "mailing" ? "Адрес прописки: " : "Адрес проживания: ") +
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
					  " (patientId,  typeId,  city,  street,  house,  apartment) "
					  " VALUES "
					  " (:patientId, "
						" (SELECT id FROM AddressType WHERE textid = :textid), "
						" :city, :street, :house, :apartment) " +
					  Patients::interfaces->db->returningSentence("id"));
			q.bindValue(":patientId", patientId);
			q.bindValue(":textid", m_adressTypeTextid);
		}

		q.bindValue(":city", nullIfEmpty(Patients::interfaces->enc->encode(city)));
		q.bindValue(":street", nullIfEmpty(Patients::interfaces->enc->encode(street)));
		q.bindValue(":house", nullIfEmpty(Patients::interfaces->enc->encode(house)));
		q.bindValue(":apartment", nullIfEmpty(Patients::interfaces->enc->encode(apartment)));

		q.exec();
		checkQuery(q);


		// обновляем ID.
		if(m_id <= 0)
			m_id = Patients::interfaces->db->lastInsertedId(&q).toInt();
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
