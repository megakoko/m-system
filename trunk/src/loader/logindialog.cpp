

#include "logindialog.h"

#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDebug>
#include <QTimer>

#include "passwords.h"


const int LoginDialog::NO_USER_ID = -1;


LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_info->clear();
	m_info->setStyleSheet("color: red");

	initConnections();
	resize(width(), 0);
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
				   Passwords::decodePassword(m_settings.value("password").toString()));
}


void LoginDialog::tryToLogin()
{
	if(connectToDatabase() == false)
	{
		showMessage(QString::fromUtf8("Подключение к базе не было установлено"));
		return;
	}

	const QString& hash = Passwords::hash(m_password->text(), salt(m_login->text()));

	QSqlQuery query;
	query.prepare(" SELECT id FROM Users "
				  " WHERE login = :login AND password = :password");
	query.bindValue(":login", m_login->text());
	query.bindValue(":password", hash);
	query.exec();

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
	query.prepare("SELECT salt FROM Users WHERE login = :login");
	query.bindValue(":login", login);
	query.exec();

	QByteArray result;
	if(query.first())
		result = query.value(0).toByteArray();

	return result;
}
