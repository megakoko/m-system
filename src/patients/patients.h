#pragma once

#include <QObject>
#include "plugininterface.h"

class Patients : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Patients();

	QString textid() const;
	PluginWidget* widget() const;

};
