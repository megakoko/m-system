#include "mainwindow.h"

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


#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

	init();
	initConnections();
	loadFiles();

}


void MainWindow::init()
{
	QSettings settings;

	m_hostname->setText(settings.value("hostname").toString());
	m_port->setValue(settings.value("port").toInt());
	m_databasename->setText(settings.value("dbname").toString());
	m_login->setText(settings.value("login").toString());

	m_toolbar->addAction("(Пере) Подключиться к серверу", this, SLOT(reconnectToDb()));
}


void MainWindow::initConnections()
{
	connect(m_updatePatientsCount, SIGNAL(clicked()), SLOT(updatePatientsCount()));
	connect(m_createPatients, SIGNAL(clicked()), SLOT(createPatients()));

}


void MainWindow::loadFiles()
{
	loadFile(":/male_firstname.txt", m_maleFirstName);
	loadFile(":/male_surname.txt", m_maleSurname);
	loadFile(":/male_patronymic.txt", m_malePatronymic);

	loadFile(":/female_firstname.txt", m_femaleFirstName);
	loadFile(":/female_surname.txt", m_femaleSurname);
	loadFile(":/female_patronymic.txt", m_femalePatronymic);

	loadFile(":/streetnames.txt", m_streetName);
}


void MainWindow::loadFile(const QString &fileName, QStringList &stringList)
{
	QFile f;
	f.setFileName(fileName);
	if(tryToOpen(f))
		readFile(f, stringList);
}


bool MainWindow::tryToOpen(QFile &f) const
{
	if(!f.open(QIODevice::ReadOnly))
		qWarning() << "Не могу открыть файл" << f.fileName();

	return f.isOpen();
}


void MainWindow::readFile(QFile &f, QStringList &stringList)
{
	QTextStream input(&f);

	QString line;
	forever
	{
		line = input.readLine();
		if(line.isNull())
			break;

		stringList << line;
	}
}







void MainWindow::showMessageInStatusBar(const QString &msg)
{
	m_statusBar->showMessage(msg, 5000);
}


void MainWindow::reconnectToDb()
{
	const QString& pw = QInputDialog::getText(this, "Пароль к БД",
											  "Введите пароль к базе данных",
											  QLineEdit::Password,
											  "mspassword");

	if(!pw.isNull())
	{
		QSqlDatabase db =	QSqlDatabase::contains() ?
							QSqlDatabase::database() :
							QSqlDatabase::addDatabase("QPSQL");

		if(db.isOpen())
			db.close();

		db.setHostName(m_hostname->text());
		db.setPort(m_port->value());
		db.setDatabaseName(m_databasename->text());

		if(db.open(m_login->text(), pw))
			showMessageInStatusBar("Подключение к БД установлено");
		else
			showMessageInStatusBar("Не удалось подключиться");
	}


	updatePatientsCount();
}


void MainWindow::updatePatientsCount()
{
	QSqlQuery q("SELECT COUNT(*) FROM Patient");
	checkQuery(q);

	q.first();
	m_patientsCount->setText(q.value(0).toString());
}


void MainWindow::createPatients()
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

		const int patientId = createPatientRecord();

		createDocumentRecord(patientId, "passport");
		createDocumentRecord(patientId, "insuranceMandatory");
		if(randomInt(2))
			createDocumentRecord(patientId, "insuranceVoluntary");


		createAddressRecord(patientId, "mailing");
		if(randomInt(5) == 0)
			createAddressRecord(patientId, "actual");


	}
	progress.setValue(patientsCount);

	q.exec("COMMIT");
	updatePatientsCount();
}


int MainWindow::createPatientRecord() const
{
	QString surname;
	QString firstname;
	QString patronymic;

	QString sextextid;
	if(randomInt(2))
	{
		randomMaleName(surname, firstname, patronymic);
		sextextid = "male";
	}
	else
	{
		randomFemaleName(surname, firstname, patronymic);
		sextextid = "female";
	}


	QSqlQuery q;
	q.prepare("INSERT INTO Patient(familyname, name, patronymic, birthday, sexid) VALUES"
			  "(:familyname, :name, :patronymic, :birthday, "
				" (SELECT id FROM Sex WHERE textid = :sextextid)) "
			  " RETURNING id");
	q.bindValue(":familyname", surname);
	q.bindValue(":name", firstname);
	q.bindValue(":patronymic", patronymic);
	q.bindValue(":birthday", randomDate());
	q.bindValue(":sextextid", sextextid);
	q.exec();
	checkQuery(q);


	q.first();
	return q.value(0).toInt();
}


void MainWindow::createDocumentRecord(const int patientId, const QString& documentTextid) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Document "
			  " (documentTypeId, patientId, serialNumber, date, givenBy) VALUES "
			  " ((SELECT id FROM DocumentType WHERE textid = :docTextid), "
			  " :patientId, :serialNumber, :date, :givenBy)");
	q.bindValue(":docTextid", documentTextid);
	q.bindValue(":patientId", patientId);
	q.bindValue(":serialNumber", QString::number(randomInt(1E4)) + ' ' +
								 QString::number(randomInt(1E6)));
	q.bindValue(":date", randomDate());
	if(documentTextid == "passport")
		q.bindValue(":givenBy", "РОВД");
	else
		q.bindValue(":givenBy", "Страховая компания");
	q.exec();
	checkQuery(q);
}


void MainWindow::createAddressRecord(const int patientId,
									 const QString& addressType) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Address "
			  " (patientId, city, street, house, apartment, typeId) VALUES"
			  " (:patientId, :city, :street, :house, :apartment, "
				" (SELECT id FROM AddressType WHERE textid = :textid))");
	q.bindValue(":patientId", patientId);
	q.bindValue(":textid", addressType);
	q.bindValue(":city", "Ижевск");
	q.bindValue(":street", randomStreetname());
	q.bindValue(":house", randomInt(250));
	q.bindValue(":apartment", randomInt(200));
	q.exec();
	checkQuery(q);
}




// ---------------------------- RANDOM функции ---------------------------

void MainWindow::initializeRandom() const
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
}


int MainWindow::randomInt(const int max) const
{
	return qrand() % max;
}


QDate MainWindow::randomDate() const
{
	return QDate(1970 + randomInt(30), randomInt(12)+1, randomInt(28)+1);
}


void MainWindow::randomMaleName(QString &surname, QString &firstname,
								QString &patronymic) const
{
	surname = m_maleSurname.at(randomInt(m_maleSurname.size()));
	firstname = m_maleFirstName.at(randomInt(m_maleFirstName.size()));
	patronymic = m_malePatronymic.at(randomInt(m_malePatronymic.size()));
}



void MainWindow::randomFemaleName(QString &surname, QString &firstname,
								QString &patronymic) const
{
	surname = m_femaleSurname.at(randomInt(m_femaleSurname.size()));
	firstname = m_femaleFirstName.at(randomInt(m_femaleFirstName.size()));
	patronymic = m_femalePatronymic.at(randomInt(m_femalePatronymic.size()));
}


QString MainWindow::randomStreetname() const
{
	return m_streetName.at(randomInt(m_streetName.size()));
}
