#pragma once

#include <QObject>
#include "plugininterface.h"

class Therapeutist : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Therapeutist();

	QString textid() const;
	PluginWidget* widget() const;

	void setInterfaces(const InterfacesPtr &interfaces);
	static InterfacesPtr interfaces;

};
