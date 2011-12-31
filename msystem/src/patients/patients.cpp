#include "patients.h"
#include <QtPlugin>
#include "mainpatientswidget.h"

InterfacesPtr Patients::interfaces;

Patients::~Patients()
{

}


QString Patients::textid() const
{
	return "patients";
}


PluginWidget* Patients::widget() const
{
	return new MainPatientsWidget();
}

void Patients::setInterfaces(const InterfacesPtr &interfaces)
{
	Patients::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, Patients)
