

#include "logindialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>

#include "mainwindow.h"
#include "database.h"
#include "macros.h"


const int LoginDialog::NO_USER_ID = -1;


LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_info->clear();
	m_info->setStyleSheet("color: red");

	initConnections();
	resize(width(), 0);


	// Кнопка сброса базы данных (будет доступна только в отладочной версии).
#ifndef QT_NO_DEBUG
	QPushButton* resetDatabaseButton =
					new QPushButton(QString::fromUtf8("Сбросить базу данных"), this);
	connect(resetDatabaseButton, SIGNAL(clicked()), SLOT(initializeDatabase()));
	m_buttonLayout->addWidget(resetDatabaseButton);
#endif

	if(connectToDatabase())
	{
		if(!databaseIsInitialized())
			initializeDatabase();
	}
	else
	{
		showMessage("Подключение к базе не было установлено. \n"
					"Проверьте настройки соединения.");
		m_ok->setEnabled(false);
#ifndef QT_NO_DEBUG
		resetDatabaseButton->setEnabled(false);
#endif
	}

}


void LoginDialog::initConnections()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(tryToLogin()));
	connect(m_quit, SIGNAL(clicked()), SLOT(reject()));

	connect(m_login, SIGNAL(returnPressed()), m_ok, SLOT(click()));
	connect(m_password, SIGNAL(returnPressed()), m_ok, SLOT(click()));
}


void LoginDialog::showMessage(const QString &message)
{
	m_info->setText(message);
	QTimer::singleShot(5000, m_info, SLOT(clear()));
}


int LoginDialog::loggedUserId() const
{
	return m_userId;
}


bool LoginDialog::connectToDatabase()
{
	if(QSqlDatabase::contains())
	{
		QSqlDatabase db = QSqlDatabase::database();

		db.close();
		QSqlDatabase::removeDatabase(db.connectionName());
	}


	const QString& dbdriver = QSettings().value("dbdriver").toString();

	QSqlDatabase db = QSqlDatabase::addDatabase(dbdriver);

	db.setHostName(m_settings.value("hostname").toString());
	db.setPort(m_settings.value("port").toInt());
	db.setDatabaseName(m_settings.value("dbname").toString());

	const bool opened = db.open(
		m_settings.value("login").toString(),
		MainWindow::interfaces->enc->decode(m_settings.value("password").toString()));

	if(opened)
		Database::setSqlDriver(dbdriver);
	else
	{
		Database::setSqlDriver(QString::null);
		QSqlDatabase::removeDatabase(db.connectionName());
	}

	return opened;
}


void LoginDialog::tryToLogin()
{
	const QString& hash = MainWindow::interfaces->enc->
						  password(m_password->text(), salt(m_login->text()));

	QSqlQuery query;
	query.prepare(" SELECT id FROM MUser "
				  " WHERE login = :login AND password = :password");
	query.bindValue(":login", m_login->text());
	query.bindValue(":password", hash);
	query.exec();
	checkQuery(query);

	if(query.first())
	{
		m_userId = query.value(0).toInt();
		accept();
	}
	else
	{
		m_userId = NO_USER_ID;
		showMessage(QString::fromUtf8("Неправильная пара логин/пароль"));
		m_ok->setDisabled(true);
		QTimer::singleShot(1000, this, SLOT(enableOkButton()));
	}
}


void LoginDialog::enableOkButton()
{
	m_ok->setEnabled(true);
}


QString LoginDialog::salt(const QString& login) const
{
	QSqlQuery query;
	query.prepare("SELECT salt FROM MUser WHERE login = :login");
	query.bindValue(":login", login);
	query.exec();
	checkQuery(query);

	QString result;
	if(query.first())
		result = query.value(0).toString();

	return result;
}


bool LoginDialog::databaseIsInitialized() const
{
	QSqlQuery q;
	return q.exec("SELECT COUNT(1) FROM MUser");
}


void LoginDialog::initializeDatabase()
{
	const int progressStart = 0;
	const int numberOfactions = 3;


	QProgressDialog d("Настройка базы данных",
					  "Пожалуйста, подождите. Выполняется настройка базы данных",
					  progressStart, progressStart + numberOfactions, this);
	d.setWindowModality(Qt::WindowModal);
	d.setCancelButton(NULL);
	d.show();


	int progress = progressStart;

	d.setValue(++progress);
	executeSqlFile(":/dbinit.sql");
	d.setValue(++progress);
	executeSqlFile(":/mkb10.sql");
	d.setValue(++progress);

	Q_ASSERT(progress == (progressStart + numberOfactions));
}


void LoginDialog::executeSqlFile(const QString &filename)
{
	QFile file(filename);
	QTextStream ts(&file);
	ts.setCodec("UTF-8");
	QSqlQuery query;


	if(!file.open(QIODevice::ReadOnly))
		qWarning() << "Cannot open file" << filename;
	else
	{
		// SQLite не переваривает несколько запросов при работе с ним через библиотеку.
		// Через консоль -- пожалуйста, а так -- нет. Поэтому весь файл разделяется на
		// отдельные SQL-команды точкой с запятой ";".ы
		QStringList statements;
		switch (MainWindow::interfaces->db->currentSqlDriver())
		{
		case DatabaseInterface::SQLITE:
			statements = convertToSqliteSyntax(ts.readAll()).split(";");
			break;
		case DatabaseInterface::PSQL:
			statements = QStringList(ts.readAll());
			break;
		default:
			qFatal("Unknown db driver");
		}

		query.exec("BEGIN");
		foreach(const QString& statement, statements)
		{
			if(!statement.simplified().isEmpty())
			{
				query.exec(statement);
				checkQuery(query);
			}
		}
		query.exec("COMMIT");
	}
}


QString LoginDialog::convertToSqliteSyntax(const QString &postgresqlSyntax)
{
	QString result(postgresqlSyntax);
	result = result.replace(QRegExp("SERIAL PRIMARY KEY", Qt::CaseInsensitive),
									"INTEGER PRIMARY KEY AUTOINCREMENT");

	return result;
}
