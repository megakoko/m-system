#include "departments.h"
#include <QtPlugin>
#include "maindepartmentswidget.h"


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

Q_EXPORT_PLUGIN2(PluginInterface, Departments)
