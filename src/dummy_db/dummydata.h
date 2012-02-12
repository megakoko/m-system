#pragma once

#include <QStringList>
#include <QDateTime>
#include <QVariant>

class QFile;


struct Name
{
	QString surname;
	QString firstname;
	QString patronymic;
};


class DummyData
{
public:
	DummyData();

	QStringList departments() const;

	// Random-функции.
	void initializeRandom() const;
	int randomInt(const int max) const;
	QDate randomDate(const int minimumYear = 1930) const;

	Name randomMaleName() const;
	Name randomFemaleName() const;
	QString randomStreetname() const;

	QVariant randomPatientId() const;

	QVariant randomTherapeutistId() const;

private:
	void loadFiles();
	void loadFile(const QString& fileName, QStringList& stringList);
	bool tryToOpen(QFile& f) const;
	void readFile(QFile& f, QStringList& stringList);

	// Dummy данные.
	QStringList m_maleFirstName;
	QStringList m_maleSurname;
	QStringList m_malePatronymic;

	QStringList m_femaleFirstName;
	QStringList m_femaleSurname;
	QStringList m_femalePatronymic;

	QStringList m_streetName;

	QStringList m_departments;

};

typedef QSharedPointer<DummyData> DummyDataPtr;
