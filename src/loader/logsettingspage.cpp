#include "logsettingspage.h"


#include <QSqlDatabase>
#include "mainwindow.h"
#include "macros.h"
#include "debuglog.h"


LogSettingsPage::LogSettingsPage(QWidget *parent)
	: AbstractSettingsPage(parent)
{
	setupUi(this);

	connect(m_enableLog, SIGNAL(clicked(bool)), SLOT(checkBoxClicked(bool)));
}



QString LogSettingsPage::pageName() const
{
	return QString::fromUtf8("Ведение лог-файла");
}


void LogSettingsPage::readSettings()
{
	m_enableLog->setChecked(m_settings.value("debugLog", false).toBool());
	m_filename->setText(m_settings.value("debugLogFilename", "log.txt").toString());

	checkBoxClicked(m_enableLog->isChecked());
}


void LogSettingsPage::saveSettings()
{
	const bool logEnabled = m_enableLog->isChecked();
	const QString& filename = m_filename->text();

	m_settings.setValue("debugLog", logEnabled);
	m_settings.setValue("debugLogFilename", filename);

	qInstallMsgHandler(logEnabled ? DebugLog::messageHandler : NULL);
	DebugLog::setFilename(filename);
}


void LogSettingsPage::checkBoxClicked(bool enableLog)
{
	m_filename->setEnabled(enableLog);
	m_filenameLabel->setEnabled(enableLog);
}
