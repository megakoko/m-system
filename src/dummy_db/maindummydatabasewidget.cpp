#include "maindummydatabasewidget.h"

#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QInputDialog>
#include <QDateTime>
#include <QFile>
#include <QProgressDialog>


#include "macros.h"
#include "dummydatabase.h"


#include <QSettings>

MainDummyDatabaseWidget::MainDummyDatabaseWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
	loadFiles();

}


void MainDummyDatabaseWidget::init()
{
	updateInformation();
}


void MainDummyDatabaseWidget::initConnections()
{
	connect(m_updateInformation, SIGNAL(clicked()), SLOT(updateInformation()));
	connect(m_createPatients, SIGNAL(clicked()), SLOT(createPatients()));
}


void MainDummyDatabaseWidget::loadFiles()
{
	loadFile(":/male_firstname.txt", m_maleFirstName);
	loadFile(":/male_surname.txt", m_maleSurname);
	loadFile(":/male_patronymic.txt", m_malePatronymic);

	loadFile(":/female_firstname.txt", m_femaleFirstName);
	loadFile(":/female_surname.txt", m_femaleSurname);
	loadFile(":/female_patronymic.txt", m_femalePatronymic);

	loadFile(":/streetnames.txt", m_streetName);
}


void MainDummyDatabaseWidget::loadFile(const QString &fileName, QStringList &stringList)
{
	QFile f;
	f.setFileName(fileName);
	if(tryToOpen(f))
		readFile(f, stringList);
}


bool MainDummyDatabaseWidget::tryToOpen(QFile &f) const
{
	if(!f.open(QIODevice::ReadOnly))
		qWarning() << "Не могу открыть файл" << f.fileName();

	return f.isOpen();
}


void MainDummyDatabaseWidget::readFile(QFile &f, QStringList &stringList)
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


QString MainDummyDatabaseWidget::encode(const QString &plaintext) const
{
	return DummyDatabase::interfaces->enc->encodeStr(plaintext);
}


QString MainDummyDatabaseWidget::encode(const QDate &plainDate) const
{
	return DummyDatabase::interfaces->enc->encodeDate(plainDate);
}


void MainDummyDatabaseWidget::updateInformation()
{
	QSqlQuery q("SELECT COUNT(*) FROM Patient");
	checkQuery(q);

	q.first();
	m_patientsCount->setText(q.value(0).toString());
}


void MainDummyDatabaseWidget::createPatients()
{
	initializeRandom();

	const int patientsCount = m_createPatientsCount->value();

	QProgressDialog progress("Создание пациентов", "Остановить", 0, patientsCount, this);
	progress.setWindowModality(Qt::WindowModal);


	QSqlQuery q("BEGIN");
	for(int i = 0; i < patientsCount; ++i)
	{
		if(progress.wasCanceled())
			break;

		if(i % 100 == 0)
		{
			progress.setValue(i);
			qApp->processEvents();
		}


		const QDate& birthday = randomDate();
		const int patientId = createPatientRecord(birthday);

		createDocumentRecord(patientId, "passport", birthday);
		createDocumentRecord(patientId, "insuranceMandatory", birthday);
		if(randomInt(2))
			createDocumentRecord(patientId, "insuranceVoluntary", birthday);


		createAddressRecord(patientId, "mailing");
		if(randomInt(5) == 0)
			createAddressRecord(patientId, "actual");


	}
	progress.setValue(patientsCount);

	q.exec("COMMIT");
	updateInformation();
}


int MainDummyDatabaseWidget::createPatientRecord(const QDate &birthday) const
{
	Name patientName;

	QString sextextid;
	if(randomInt(2))
	{
		patientName = randomMaleName();
		sextextid = "male";
	}
	else
	{
		patientName = randomFemaleName();
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


void MainDummyDatabaseWidget::createDocumentRecord(const int patientId,
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
	q.bindValue(":serialNumber", encode(QString::number(randomInt(1E4)) + ' ' +
										QString::number(randomInt(1E6))));
	q.bindValue(":date", encode(randomDate(patientBirthday.year() + 14)));
	if(documentTextid == "passport")
		q.bindValue(":givenBy", encode("РОВД"));
	else
		q.bindValue(":givenBy", encode("Страховая компания"));
	q.exec();
	checkQuery(q);
}


void MainDummyDatabaseWidget::createAddressRecord(const int patientId,
									 const QString& addressType) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Address "
			  " (patientId, city, street, house, apartment, typeId) VALUES"
			  " (:patientId, :city, :street, :house, :apartment, "
				" (SELECT id FROM AddressType WHERE textid = :textid))");
	q.bindValue(":patientId", patientId);
	q.bindValue(":textid", addressType);
	q.bindValue(":city", encode("Ижевск"));
	q.bindValue(":street", encode(randomStreetname()));
	q.bindValue(":house", encode(QString::number(randomInt(250))));
	q.bindValue(":apartment", encode(QString::number(randomInt(200))));
	q.exec();
	checkQuery(q);
}




// ---------------------------- RANDOM функции ---------------------------

void MainDummyDatabaseWidget::initializeRandom() const
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
}


int MainDummyDatabaseWidget::randomInt(const int max) const
{
	return qrand() % max;
}


QDate MainDummyDatabaseWidget::randomDate(const int minimumYear) const
{
	const int maximumAge = qMax(1990 - minimumYear, 14);
	return QDate(minimumYear + randomInt(maximumAge), randomInt(12)+1, randomInt(28)+1);
}


Name MainDummyDatabaseWidget::randomMaleName() const
{
	Name name = {
		m_maleSurname.at(randomInt(m_maleSurname.size())),
		m_maleFirstName.at(randomInt(m_maleFirstName.size())),
		m_malePatronymic.at(randomInt(m_malePatronymic.size()))
	};
	return name;
}



Name MainDummyDatabaseWidget::randomFemaleName() const
{
	Name name = {
		m_femaleSurname.at(randomInt(m_femaleSurname.size())),
		m_femaleFirstName.at(randomInt(m_femaleFirstName.size())),
		m_femalePatronymic.at(randomInt(m_femalePatronymic.size())),
	};
	return name;
}


QString MainDummyDatabaseWidget::randomStreetname() const
{
	return m_streetName.at(randomInt(m_streetName.size()));
}
