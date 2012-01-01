#include "departments.h"
#include <QtPlugin>
#include "maindepartmentswidget.h"



InterfacesPtr Departments::interfaces;


Departments::~Departments()
{

}


QString Departments::textid() const
{
	return "departments";
}


PluginWidget* Departments::widget() const
{
	return new MainDepartmentsWidget();
}


void Departments::setInterfaces(const InterfacesPtr &interfaces)
{
	Departments::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, Departments)
