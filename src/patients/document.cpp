#include "document.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#include "patients.h"
#include "macros.h"


const QMap<int, QString> Document::m_documentTypeToDocumentName =
	Document::mapInitializer();


QMap<int, QString> Document::mapInitializer()
{
	QMap<int, QString> result;

	QSqlQuery q("SELECT id, name FROM DocumentType ORDER BY id");
	while(q.next())
		result[q.value(0).toInt()] = q.value(1).toString();

	return result;
}




Document::Document()
	: documentTypeId(-1)
	, m_id(-1)
{
}


Document::Document(const QSqlRecord &rec)
	: documentTypeId(rec.value("documentTypeId").toInt())
	, serialNumber(Patients::interfaces->enc->decodeStr(rec.value("serialNumber").toString()))
	, date(Patients::interfaces->enc->decodeDate(rec.value("date").toString()))
	, givenBy(Patients::interfaces->enc->decodeStr(rec.value("givenBy").toString()))
	, m_id(rec.value("id").toInt())
{

}


QString Document::documentName() const
{
	Q_ASSERT(m_documentTypeToDocumentName.contains(documentTypeId));
	return m_documentTypeToDocumentName.value(documentTypeId);
}


void Document::save(const int patientId) const
{
	QSqlQuery q;
	if(m_id > 0)
	{
		q.prepare(" UPDATE Document SET "
				  " documentTypeId = :documentTypeId, "
				  " serialNumber = :serialNumber, "
				  " date = :date, "
				  " givenBy = :givenBy "
				  " WHERE id = :id ");
		q.bindValue(":id", m_id);
	}
	else
	{
		q.prepare(" INSERT INTO Document "
				  " (patientId,  documentTypeId,  serialNumber,  date,  givenBy)"
		   " VALUES(:patientId, :documentTypeId, :serialNumber, :date, :givenBy)");
		q.bindValue(":patientId", patientId);
	}
	q.bindValue(":documentTypeId", documentTypeId);
	q.bindValue(":serialNumber", Patients::interfaces->enc->encodeStr(serialNumber));
	q.bindValue(":date", Patients::interfaces->enc->encodeDate(date));
	q.bindValue(":givenBy", Patients::interfaces->enc->encodeStr(givenBy));

	q.exec();
	checkQuery(q);
}


void Document::deleteDocument() const
{
	if(m_id > 0)
	{
		QSqlQuery q;
		q.prepare("DELETE FROM Document WHERE id = :id");
		q.bindValue(":id", m_id);

		q.exec();
		checkQuery(q);
	}
}
