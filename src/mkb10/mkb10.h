#pragma once

#include <QtPlugin>
#include <QObject>
#include "plugininterface.h"

class PluginWidget;

class Mkb10 : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Mkb10();

	QString name() const;
	QString textid() const;
	PluginWidget* widget() const;
	QWidget* settingsPage() const;
};
