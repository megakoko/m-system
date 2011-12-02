#include "mkb10.h"
#include <QtPlugin>
#include "mainmkb10widget.h"


Mkb10::~Mkb10()
{

}


QString Mkb10::name() const
{
	return QString::fromUtf8("Справочная система МКБ-10");
}


QString Mkb10::textid() const
{
	return "mkb10";
}


PluginWidget* Mkb10::widget() const
{
	return new MainMkb10Widget();
}


QWidget* Mkb10::settingsPage() const
{
	return NULL;
}


Q_EXPORT_PLUGIN2(PluginInterface, Mkb10)

