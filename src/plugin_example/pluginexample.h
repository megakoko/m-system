#pragma once

#include <QObject>
#include "plugininterface.h"

class PluginExample : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~PluginExample();

	QString textid() const;
	PluginWidget* widget() const;

};
