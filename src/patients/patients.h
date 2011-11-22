#pragma once

#include <QObject>
#include "../loader/plugininterface.h"

class Patients : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
public:
	~Patients();

	QString name() const;
	QString textid() const;
	PluginWidget* widget() const;
	QWidget* settingsPage() const;
};
