#include "mkb10.h"
#include <QtPlugin>
#include "mainmkb10widget.h"


Mkb10::~Mkb10()
{

}


QString Mkb10::textid() const
{
	return "mkb10";
}


PluginWidget* Mkb10::widget() const
{
	return new MainMkb10Widget();
}

Q_EXPORT_PLUGIN2(PluginInterface, Mkb10)
