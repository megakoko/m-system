#include "databasesettingspage.h"


#include <QSqlDatabase>
#include "mainwindow.h"
#include "macros.h"


DatabaseSettingsPage::DatabaseSettingsPage(QWidget *parent)
	: AbstractSettingsPage(parent)
{
	setupUi(this);


	if(QSqlDatabase::drivers().contains("QSQLITE"))
		m_databaseType->addItem("SQLite", "QSQLITE");

	if(QSqlDatabase::drivers().contains("QPSQL"))
		m_databaseType->addItem("PostgreSQL", "QPSQL");


	connect(m_databaseType,
			SIGNAL(currentIndexChanged(int)),
			SLOT(databaseDriverChanged()));

	databaseDriverChanged();
}



QString DatabaseSettingsPage::pageName() const
{
	return QString::fromUtf8("База данных");
}


void DatabaseSettingsPage::readSettings()
{
	m_databaseType->setCurrentIndex(
				m_databaseType->findData(m_settings.value("dbdriver", "QSQLITE")));
	m_hostname->setText(m_settings.value("hostname").toString());
	m_port->setText(m_settings.value("port").toString());
	m_databasename->setText(m_settings.value("dbname").toString());
	m_login->setText(m_settings.value("login").toString());
	m_password->setText(MainWindow::interfaces->enc->
						decodeStr(m_settings.value("password").toString()));
}


void DatabaseSettingsPage::saveSettings()
{
	m_settings.setValue("dbdriver",
						m_databaseType->itemData(m_databaseType->currentIndex()));
	m_settings.setValue("hostname", m_hostname->text());
	m_settings.setValue("port", m_port->text());
	m_settings.setValue("dbname", m_databasename->text());
	m_settings.setValue("login", m_login->text());
	m_settings.setValue("password", MainWindow::interfaces->enc->
						encodeStr(m_password->text()));
}


void DatabaseSettingsPage::databaseDriverChanged()
{
	const bool usingSqlite =
			(m_databaseType->itemData(m_databaseType->currentIndex()).toString()
			 ==
			 "QSQLITE");

	m_port->setHidden(usingSqlite);
	m_portLabel->setHidden(usingSqlite);
	m_hostname->setHidden(usingSqlite);
	m_hostnameLabel->setHidden(usingSqlite);
	m_login->setHidden(usingSqlite);
	m_loginLabel->setHidden(usingSqlite);
	m_password->setHidden(usingSqlite);
	m_passwordLabel->setHidden(usingSqlite);
}
