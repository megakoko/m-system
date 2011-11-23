#pragma once

#include <QString>
#include <QMap>
#include <QDate>

class QSqlRecord;

class Document
{
public:
    Document();
	Document(const QSqlRecord& rec);

	void save(const int patientId) const;
	void deleteDocument() const;

	QString documentName() const;

	int documentTypeId;
	QString serialNumber;
	QString number;
	QDate date;
	QString givenBy;

private:
	int m_id;

	static QMap<int, QString> mapInitializer();
	static const QMap<int, QString> m_documentTypeToDocumentName;

};
