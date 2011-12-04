
#pragma once


class QString;
class QStringList;
class PluginWidget;
class QWidget;

class PluginInterface {
public:
	virtual ~PluginInterface() {}

	virtual QString textid() const = 0;
	virtual PluginWidget* widget() const = 0;

};

Q_DECLARE_INTERFACE(PluginInterface,
					"com.msystem.Application.PluginInterface/1.0"
				   )
