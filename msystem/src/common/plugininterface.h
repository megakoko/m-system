
#pragma once
#include "interfaces.h"

class QString;
class QStringList;
class PluginWidget;
class QWidget;

class PluginInterface {
public:
	virtual ~PluginInterface() {}

	virtual QString textid() const = 0;
	virtual PluginWidget* widget() const = 0;

	virtual void setInterfaces(const InterfacesPtr& interfaces) {
		Q_UNUSED(interfaces);
	}

};

Q_DECLARE_INTERFACE(PluginInterface, "com.msystem.Application.PluginInterface/1.0")
