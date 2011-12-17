#include "patients.h"
#include <QtPlugin>
#include "mainpatientswidget.h"


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

Q_EXPORT_PLUGIN2(PluginInterface, Patients)
