

#include "logindialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>

#include "passwords.h"
#include "encoding.h"
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

	if(connectToDatabase() && !databaseIsInitialized())
		initializeDatabase();
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
	QSqlDatabase db =	QSqlDatabase::contains() ?
						QSqlDatabase::database() :
						QSqlDatabase::addDatabase("QPSQL");

	if(db.isOpen())
		db.close();

	db.setHostName(m_settings.value("hostname").toString());
	db.setPort(m_settings.value("port").toInt());
	db.setDatabaseName(m_settings.value("dbname").toString());


	return db.open(m_settings.value("login").toString(),
				   Encoding::decodePassword(m_settings.value("password").toString()));
}


void LoginDialog::tryToLogin()
{
	if(connectToDatabase() == false)
	{
		showMessage(QString::fromUtf8("Подключение к базе не было установлено"));
		return;
	}


	const QString& hash = Passwords::hash(m_password->text(), salt(m_login->text()));


	// TODO: select count(*) from muser
	//        where login='admin' and md5(md5('pw') || salt) = password;
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


QByteArray LoginDialog::salt(const QString& login) const
{
	QSqlQuery query;
	query.prepare("SELECT salt FROM MUser WHERE login = :login");
	query.bindValue(":login", login);
	query.exec();
	checkQuery(query);

	QByteArray result;
	if(query.first())
		result = query.value(0).toByteArray();

	return result;
}


bool LoginDialog::databaseIsInitialized() const
{
	QSqlQuery q;
	return q.exec("SELECT COUNT(1) FROM MUser");
}


void LoginDialog::initializeDatabase()
{
	const QString& title = QString::fromUtf8("Настройка базы данных");
	const QString& msg =
			QString::fromUtf8("Пожалуйста, подождите. "
							  "Выполняется настройка базы данных");
	QProgressDialog d(title, msg, 0, 2, this);
	d.setWindowModality(Qt::WindowModal);

	d.setCancelButton(NULL);
	d.show();


	int progress = 0;


	QFile file;
	QTextStream ts(&file);
	QSqlQuery query;


	file.setFileName(":/dbinit.sql");
	if(!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Cant open dbinit.sql file";
		return;
	}
	query.exec(ts.readAll());
	checkQuery(query);

	d.setValue(++progress);


	file.close();
	file.setFileName(":/mkb10.sql");
	if(!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Cant open mkb10.sql file";
		return;
	}
	query.exec(ts.readAll());
	checkQuery(query);

	d.setValue(++progress);
}
