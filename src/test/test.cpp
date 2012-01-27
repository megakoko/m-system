#include "test.h"

#include <QString>
#include <QtPlugin>

#include "maintestwidget.h"


Test::~Test()
{

}


QString Test::textid() const
{
	return "test";
}

PluginWidget* Test::widget() const
{
	return new MainTestWidget();
}

Q_EXPORT_PLUGIN2(PluginInterface, Test)
