#pragma once

#include <QObject>
#include "plugininterface.h"

class Users : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Users();

	QString textid() const;
	PluginWidget* widget() const;

	void setInterfaces(const InterfacesPtr &interfaces);
	static InterfacesPtr interfaces;

};
