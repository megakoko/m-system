#pragma once

#include <QObject>
#include "plugininterface.h"

class DummyDatabase : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~DummyDatabase();

	QString textid() const;
	PluginWidget* widget() const;

	void setInterfaces(const InterfacesPtr &interfaces);
	static InterfacesPtr interfaces;

};
