#include "dummydata.h"

#include <QFile>
#include <QTextStream>

#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"
#include <QDebug>


DummyData::DummyData()
{
	loadFiles();

	initializeRandom();
}


void DummyData::loadFiles()
{
	loadFile(":/male_firstname.txt", m_maleFirstName);
	loadFile(":/male_surname.txt", m_maleSurname);
	loadFile(":/male_patronymic.txt", m_malePatronymic);

	loadFile(":/female_firstname.txt", m_femaleFirstName);
	loadFile(":/female_surname.txt", m_femaleSurname);
	loadFile(":/female_patronymic.txt", m_femalePatronymic);

	loadFile(":/streetnames.txt", m_streetName);

	loadFile(":/departments.txt", m_departments);
}


void DummyData::loadFile(const QString &fileName, QStringList &stringList)
{
	QFile f;
	f.setFileName(fileName);
	if(tryToOpen(f))
		readFile(f, stringList);
}


bool DummyData::tryToOpen(QFile &f) const
{
	if(!f.open(QIODevice::ReadOnly))
		qWarning() << "Не могу открыть файл" << f.fileName();

	return f.isOpen();
}


void DummyData::readFile(QFile &f, QStringList &stringList)
{
	QTextStream input(&f);
	input.setCodec("UTF-8");

	QString line;
	forever
	{
		line = input.readLine();
		if(line.isNull())
			break;

		stringList << line;
	}
}


void DummyData::initializeRandom() const
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
}


QStringList DummyData::departments() const
{
	return m_departments;
}


QVariant DummyData::randomPatientId() const
{
	QSqlQuery q("SELECT id FROM Patient ORDER BY random() LIMIT 1");

	QVariant id;
	if(q.first())
		id = q.value(0);

	return id;
}


int DummyData::randomInt(const int max) const
{
	return qrand() % max;
}


QDate DummyData::randomDate(const int minimumYear) const
{
	const int maximumAge = qMax(1990 - minimumYear, 14);
	return QDate(minimumYear + randomInt(maximumAge), randomInt(12)+1, randomInt(28)+1);
}


Name DummyData::randomMaleName() const
{
	Name name = {
		m_maleSurname.at(randomInt(m_maleSurname.size())),
		m_maleFirstName.at(randomInt(m_maleFirstName.size())),
		m_malePatronymic.at(randomInt(m_malePatronymic.size()))
	};
	return name;
}



Name DummyData::randomFemaleName() const
{
	Name name = {
		m_femaleSurname.at(randomInt(m_femaleSurname.size())),
		m_femaleFirstName.at(randomInt(m_femaleFirstName.size())),
		m_femalePatronymic.at(randomInt(m_femalePatronymic.size())),
	};
	return name;
}


QString DummyData::randomStreetname() const
{
	return m_streetName.at(randomInt(m_streetName.size()));
}
