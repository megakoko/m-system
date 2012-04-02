#include "dummypatients.h"

#include <QApplication>

#include <QProgressDialog>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"
#include "dummydatabase.h"


DummyPatients::DummyPatients(DummyDataPtr dummyData)
	: m_dummyData(dummyData)
{

}


QString DummyPatients::encode(const QString &plaintext) const
{
	return DummyDatabase::interfaces->enc->encodeStr(plaintext);
}


QString DummyPatients::encode(const QDate &plainDate) const
{
	return DummyDatabase::interfaces->enc->encodeDate(plainDate);
}


void DummyPatients::createPatients(const int count)
{
	QProgressDialog progress("Создание пациентов", "Остановить", 0, count);
	progress.setWindowModality(Qt::WindowModal);


	QSqlQuery q("BEGIN");
	for(int i = 0; i < count; ++i)
	{
		if(progress.wasCanceled())
			break;

		if(i % 100 == 0)
		{
			progress.setValue(i);
			qApp->processEvents();
		}


		const QDate& birthday = m_dummyData->randomDate();
		const int patientId = createPatientRecord(birthday);

		createDocumentRecord(patientId, "passport", birthday);
		createDocumentRecord(patientId, "insuranceMandatory", birthday);
		if(m_dummyData->randomInt(2))
			createDocumentRecord(patientId, "insuranceVoluntary", birthday);


		createAddressRecord(patientId, "mailing");
		if(m_dummyData->randomInt(5) == 0)
			createAddressRecord(patientId, "actual");
	}
	progress.setValue(count);

	q.exec("COMMIT");
}


int DummyPatients::createPatientRecord(const QDate &birthday) const
{
	Name patientName;

	QString sextextid;
	if(m_dummyData->randomInt(2))
	{
		patientName = m_dummyData->randomMaleName();
		sextextid = "male";
	}
	else
	{
		patientName = m_dummyData->randomFemaleName();
		sextextid = "female";
	}


	QSqlQuery q;
	q.prepare("INSERT INTO Patient(familyname, name, patronymic, birthday, sexid) VALUES"
			  "(:familyname, :name, :patronymic, :birthday, "
				" (SELECT id FROM Sex WHERE textid = :sextextid)) " +
			  DummyDatabase::interfaces->db->returningSentence("id"));
	q.bindValue(":familyname", encode(patientName.surname));
	q.bindValue(":name", encode(patientName.firstname));
	q.bindValue(":patronymic", encode(patientName.patronymic));
	q.bindValue(":birthday", encode(birthday));
	q.bindValue(":sextextid", sextextid);
	q.exec();
	checkQuery(q);


	q.first();
	return DummyDatabase::interfaces->db->lastInsertedId(&q).toInt();
}


void DummyPatients::createDocumentRecord(const int patientId,
												   const QString& documentTextid,
												   const QDate &patientBirthday) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Document "
			  " (documentTypeId, patientId, serialNumber, date, givenBy) VALUES "
			  " ((SELECT id FROM DocumentType WHERE textid = :docTextid), "
			  " :patientId, :serialNumber, :date, :givenBy)");
	q.bindValue(":docTextid", documentTextid);
	q.bindValue(":patientId", patientId);
	q.bindValue(":serialNumber", encode(QString::number(m_dummyData->randomInt(1E4)) + ' ' +
										QString::number(m_dummyData->randomInt(1E6))));
	q.bindValue(":date", encode(m_dummyData->randomDate(patientBirthday.year() + 14)));
	if(documentTextid == "passport")
		q.bindValue(":givenBy", encode("РОВД"));
	else
		q.bindValue(":givenBy", encode("Страховая компания"));
	q.exec();
	checkQuery(q);
}


void DummyPatients::createAddressRecord(const int patientId,
									 const QString& addressType) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Address "
			  " (patientId, region, city, street, house, apartment, typeId) VALUES"
			  " (:patientId, :region, :city, :street, :house, :apartment, "
				" (SELECT id FROM AddressType WHERE textid = :textid))");
	q.bindValue(":patientId", patientId);
	q.bindValue(":textid", addressType);
	q.bindValue(":region", encode("Удмуртия"));
	q.bindValue(":city", encode("Ижевск"));
	q.bindValue(":street", encode(m_dummyData->randomStreetname()));
	q.bindValue(":house", encode(QString::number(m_dummyData->randomInt(250))));
	q.bindValue(":apartment", encode(QString::number(m_dummyData->randomInt(200))));
	q.exec();
	checkQuery(q);
}
