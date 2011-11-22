#include "patients.h"
#include <QtPlugin>
#include "mainpatientswidget.h"


Patients::~Patients()
{

}


QString Patients::name() const
{
	return QString::fromUtf8("Пациенты");
}


QString Patients::textid() const
{
	return "patients";
}


PluginWidget* Patients::widget() const
{
	return new MainPatientsWidget();
}


QWidget* Patients::settingsPage() const
{
	return NULL;
}


Q_EXPORT_PLUGIN2(PluginInterface, Patients)

