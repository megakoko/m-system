
#include "databasesettingspage.h"
#include "passwords.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFile>
#include <QMessageBox>

#include "mainwindow.h"
#include "macros.h"


DatabaseSettingsPage::DatabaseSettingsPage(QWidget *parent)
	: AbstractSettingsPage(parent)
{
	setupUi(this);

	AbstractSettingsPage::connect(m_resetDatabase, SIGNAL(clicked()), this, SLOT(resetDatabase()));
}



QString DatabaseSettingsPage::pageName() const
{
	return QString::fromUtf8("База данных");
}


void DatabaseSettingsPage::readSettings()
{
	m_hostname->setText(m_settings.value("hostname").toString());
	m_port->setText(m_settings.value("port").toString());
	m_databasename->setText(m_settings.value("dbname").toString());
	m_login->setText(m_settings.value("login").toString());
	m_password->setText(Passwords::decodePassword(m_settings.value("password").toString()));
}


void DatabaseSettingsPage::saveSettings()
{
	m_settings.setValue("hostname", m_hostname->text());
	m_settings.setValue("port", m_port->text());
	m_settings.setValue("dbname", m_databasename->text());
	m_settings.setValue("login", m_login->text());
	m_settings.setValue("password", Passwords::encodePassword(m_password->text()));
}


void DatabaseSettingsPage::resetDatabase()
{
	// TODO: check if user is admin.


	const QString& title = QString::fromUtf8("Сброс базы данных");
	const QString& description =
			QString::fromUtf8("После сброса базы данных программа закроется. "
							  "Вы хотите продолжить?");

	const int rc = QMessageBox::question(this, title,
										 description, QMessageBox::Yes | QMessageBox::No);

	if(rc == QMessageBox::No)
		return;


	if(!QSqlDatabase::database().isOpen())
	{
		// QMessageBox TODO
		return;
	}

	QFile file(":/dbinit.sql");
	if(!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Cant open dbinit.sql file";
		return;
	}

	QTextStream ts(&file);

	QSqlQuery query;
	query.exec(ts.readAll());
	checkQuery(query);

	/*
	QMap<QString, QString> map = MainWindow::pluginTextidToDescription();
	QMap<QString, QString>::const_iterator i = map.constBegin();


	query.prepare(" INSERT INTO Plugins (textid, name) "
				  " VALUES (:textid, :name) ");
	while(i != map.constEnd())
	{
		query.bindValue(":textid", i.key());
		query.bindValue(":name", i.value());
		query.exec();
		++i;
	}
	*/


	qApp->exit(0);
}
