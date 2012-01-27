#include "users.h"


#include <QString>
#include <QtPlugin>

#include "mainuserswidget.h"


InterfacesPtr Users::interfaces;


Users::~Users()
{

}


QString Users::textid() const
{
	return "users";
}

PluginWidget* Users::widget() const
{
	return new MainUsersWidget();
}

void Users::setInterfaces(const InterfacesPtr &interfaces)
{
	Users::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, Users)
