
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QDebug>

#include "abstractsettingspage.h"



class TestSettings : public AbstractSettingsPage
{
public:
	TestSettings(QWidget* parent)
		: AbstractSettingsPage(parent)
	{
		new QPushButton(this);
	}

	~TestSettings()
	{
	}

	QString pageName() const {
		return "Test";
	}
	void readSettings() { qDebug() << "test read"; }
	void saveSettings() { qDebug() << "test save"; }


};
