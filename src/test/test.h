#pragma once

#include <QObject>
#include "plugininterface.h"

class Test : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Test();

	QString textid() const;
	PluginWidget* widget() const;

};
