#pragma once

#include <QObject>
#include "plugininterface.h"

class Mkb10 : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Mkb10();

	QString textid() const;
	PluginWidget* widget() const;

	void setInterfaces(const InterfacesPtr &interfaces);
	static InterfacesPtr interfaces;

};
