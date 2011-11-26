#pragma once

#include <QObject>
#include "plugininterface.h"

class Administrator : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Administrator();

	QString name() const;
	QString textid() const;
	PluginWidget* widget() const;
	QWidget* settingsPage() const;


};
