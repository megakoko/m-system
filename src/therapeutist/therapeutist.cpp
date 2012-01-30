#include "therapeutist.h"


#include <QString>
#include <QtPlugin>

#include "maintherapeutistwidget.h"


InterfacesPtr Therapeutist::interfaces;


Therapeutist::~Therapeutist()
{

}


QString Therapeutist::textid() const
{
	return "therapeutist";
}

PluginWidget* Therapeutist::widget() const
{
	return new MainTherapeutistWidget();
}

void Therapeutist::setInterfaces(const InterfacesPtr &interfaces)
{
	Therapeutist::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, Therapeutist)
