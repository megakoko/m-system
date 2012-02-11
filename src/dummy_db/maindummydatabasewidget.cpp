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
	connect(m_createStaff, SIGNAL(clicked()), SLOT(createStaff()));
	connect(m_createDepartments, SIGNAL(clicked()), SLOT(createDepartments()));
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

	loadFile(":/departments.txt", m_departments);
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
	m_patientsCount->setNum(q.value(0).toInt());

	q.exec("SELECT COUNT(*) FROM Staff");
	checkQuery(q);
	q.first();
	m_staffCount->setNum(q.value(0).toInt());
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


void MainDummyDatabaseWidget::createStaff()
{
	initializeRandom();

	QSqlQuery q;
	q.prepare(" INSERT INTO Staff "
			  " (familyName, name, patronymic, birthday, specialization) "
			  " VALUES (?, ?, ?, ?, ?)");


	for(int i = 0; i < m_createStaffCount->value(); ++i)
	{
		const Name& name = randomInt(2) ? randomMaleName() : randomFemaleName();
		const QDate& birthday = randomDate();
		const QString& specialization = "";

		q.addBindValue(name.surname);
		q.addBindValue(name.firstname);
		q.addBindValue(name.patronymic);
		q.addBindValue(birthday);
		q.addBindValue(specialization);
		q.exec();
		checkQuery(q);
	}

	updateInformation();
}


QVariantList MainDummyDatabaseWidget::positionIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Position");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


QVariantList MainDummyDatabaseWidget::staffIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Staff");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


void MainDummyDatabaseWidget::createPositions()
{
	static const QStringList predefinedPositions =
			QStringList() << "Врач" << "Лаборант" << "Медсестра";

	QSet<QString> positionsInDatabase;

	QSqlQuery q;

	q.exec("SELECT name FROM Position");
	checkQuery(q);
	while(q.next())
		positionsInDatabase << q.value(0).toString();


	q.prepare("INSERT INTO Position(name) VALUES(?)");
	foreach(const QString& position, predefinedPositions)
		if(!positionsInDatabase.contains(position))
		{
			q.addBindValue(position);
			q.exec();
			checkQuery(q);
		}
}


void MainDummyDatabaseWidget::createDepartmentStaffPositions(const QVariant departmentId,
															 const QVariant headOfDepartmentId,
															 const QVariantList &staffIds,
															 const QVariantList &positionIds)
{
	if(!staffIds.isEmpty() && !positionIds.isEmpty())
	{
		QSqlQuery q;
		q.prepare(" INSERT INTO DepartmentStaffPosition "
				  " (departmentId, staffId, positionId) "
				  " VALUES(?, ?, ?) ");


		const int numberOfPeopleInDepartment = staffIds.size() / 10;
		for(int i = 0; i < numberOfPeopleInDepartment - 1; ++i)
		{
			q.addBindValue(departmentId);
			q.addBindValue(staffIds.at(randomInt(staffIds.size())));
			q.addBindValue(positionIds.at(randomInt(positionIds.size())));
			q.exec();
			checkQuery(q);
		}


		// Глава отделения пусть тоже какую-то должность занимает помимо должности главы
		// отделения.
		q.addBindValue(departmentId);
		q.addBindValue(headOfDepartmentId);
		q.addBindValue(positionIds.at(randomInt(positionIds.size())));
		q.exec();
		checkQuery(q);
	}
}


void MainDummyDatabaseWidget::createDepartments()
{
	createPositions();

	const QVariantList staffIds = staffIdsFromDatabase();
	const QVariantList positionIds = positionIdsFromDatabase();


	if(staffIds.isEmpty() || positionIds.isEmpty())
		return;


	QSqlQuery q;

	q.exec("SELECT name FROM Department");
	checkQuery(q);

	QSet<QString> departmentsInDatabase;
	while(q.next())
		departmentsInDatabase << q.value(0).toString();


	q.prepare(" INSERT INTO Department "
			  " (name, shortName, typeid, headOfDepartmentId) "
			  " VALUES(?, ?, (SELECT id FROM DepartmentType WHERE textid = ?), ?)" +
			  DummyDatabase::interfaces->db->returningSentence("id"));
	for(int i = 0; i < m_departments.size(); ++i)
	{
		const QString& name = m_departments.at(i);
		if(!departmentsInDatabase.contains(name) && !name.isEmpty())
		{
			const QString& type = randomInt(2) ? "clinic" : "hospital";
			const QVariant headId = staffIds.at(randomInt(staffIds.size()));


			q.addBindValue(name);
			q.addBindValue(name[0].toUpper());
			q.addBindValue(type);
			q.addBindValue(headId);

			q.exec();
			checkQuery(q);

			const QVariant& departmentId = DummyDatabase::interfaces->db->lastInsertedId(&q);
			createDepartmentStaffPositions(departmentId, headId, staffIds, positionIds);
		}
	}
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
