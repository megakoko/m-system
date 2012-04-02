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
	: region(Patients::interfaces->enc->decodeStr(rec.value("region").toString()))
	, city(Patients::interfaces->enc->decodeStr(rec.value("city").toString()))
	, street(Patients::interfaces->enc->decodeStr(rec.value("street").toString()))
	, house(Patients::interfaces->enc->decodeStr(rec.value("house").toString()))
	, apartment(Patients::interfaces->enc->decodeStr(rec.value("apartment").toString()))
	, m_adressTypeTextid(rec.value("textid").toBool())
	, m_id(rec.value("id").toInt())
{

}


QString Address::toString() const
{
	QStringList result;

	if(!region.isNull())
		result << region;

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
					  " region = :region, "
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
					  " (patientId,  typeId,  region,  city,  street,  house,  apartment) "
					  " VALUES "
					  " (:patientId, "
						" (SELECT id FROM AddressType WHERE textid = :textid), "
						" :region, :city, :street, :house, :apartment) " +
					  Patients::interfaces->db->returningSentence("id"));
			q.bindValue(":patientId", patientId);
			q.bindValue(":textid", m_adressTypeTextid);
		}

		q.bindValue(":region", nullIfEmpty(Patients::interfaces->enc->encodeStr(region)));
		q.bindValue(":city", nullIfEmpty(Patients::interfaces->enc->encodeStr(city)));
		q.bindValue(":street", nullIfEmpty(Patients::interfaces->enc->encodeStr(street)));
		q.bindValue(":house", nullIfEmpty(Patients::interfaces->enc->encodeStr(house)));
		q.bindValue(":apartment", nullIfEmpty(Patients::interfaces->enc->encodeStr(apartment)));

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
	return	region.isNull() &&
			city.isNull() &&
			street.isNull() &&
			house.isNull() &&
			apartment.isNull();
}
