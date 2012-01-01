
#pragma once

#include "ui_databasesettingspage.h"
#include "abstractsettingspage.h"


class DatabaseSettingsPage : public AbstractSettingsPage, private Ui::DatabaseSettingsPage
{
	Q_OBJECT
public:
	DatabaseSettingsPage(QWidget* parent);

	QString pageName() const;
	void readSettings();
	void saveSettings();

};
