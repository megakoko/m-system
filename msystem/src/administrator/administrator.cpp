#include "administrator.h"


#include <QString>
#include <QtPlugin>

#include "mainadminwidget.h"


InterfacesPtr Administrator::interfaces;


Administrator::~Administrator()
{

}


QString Administrator::textid() const
{
	return "admin";
}

PluginWidget* Administrator::widget() const
{
	return new MainAdminWidget();
}

void Administrator::setInterfaces(const InterfacesPtr &interfaces)
{
	Administrator::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, Administrator)
