#include "pluginexample.h"

#include <QString>
#include <QtPlugin>

#include "pluginexamplewidget.h"


PluginExample::~PluginExample()
{

}


QString PluginExample::textid() const
{
	return "plugin_example";
}

PluginWidget* PluginExample::widget() const
{
	return new PluginExampleWidget();
}

Q_EXPORT_PLUGIN2(PluginInterface, PluginExample)
