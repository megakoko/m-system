
#include "databasesettingspage.h"
#include "passwords.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "mainwindow.h"
#include "macros.h"


DatabaseSettingsPage::DatabaseSettingsPage(QWidget *parent)
	: AbstractSettingsPage(parent)
{
	setupUi(this);
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

