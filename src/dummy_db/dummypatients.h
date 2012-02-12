#pragma once

#include "dummydata.h"

class DummyPatients
{
public:
	DummyPatients(DummyDataPtr dummyData);

	void createPatients(const int count);

private:
	// Возвращает зашифрованную с помощью CryptoPP строку.
	QString encode(const QString& plaintext) const;
	// Возвращает зашифрованную с помощью CryptoPP дату.
	QString encode(const QDate& plainDate) const;


	// Возвращает ID пациента.
	int createPatientRecord(const QDate& birthday) const;
	void createDocumentRecord(const int patientId,
							  const QString& documentTextid,
							  const QDate& patientBirthday) const;
	void createAddressRecord(const int patientId, const QString& addressType) const;

	DummyDataPtr m_dummyData;

};
