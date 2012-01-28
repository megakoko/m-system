

#pragma once

#include <QWidget>
#include <QString>
#include <QSettings>


class AbstractSettingsPage : public QWidget
{
public:
	AbstractSettingsPage(QWidget* parent)
		: QWidget(parent)
	{		}
	virtual ~AbstractSettingsPage() {}
	virtual QString pageName() const = 0;
	virtual void readSettings() = 0;
	virtual void saveSettings() = 0;

protected:
	QSettings m_settings;
};

