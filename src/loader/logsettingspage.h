
#pragma once

#include "ui_logsettingspage.h"
#include "abstractsettingspage.h"


class LogSettingsPage : public AbstractSettingsPage, private Ui::LogSettingsPage
{
	Q_OBJECT
public:
	LogSettingsPage(QWidget* parent);

	QString pageName() const;
	void readSettings();
	void saveSettings();

private slots:
	void checkBoxClicked(bool enableLog);

};
