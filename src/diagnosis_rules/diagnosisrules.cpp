#include "diagnosisrules.h"

#include <QtPlugin>
#include "maindiagnosiswidget.h"



InterfacesPtr DiagnosisRules::interfaces;


DiagnosisRules::~DiagnosisRules()
{

}


QString DiagnosisRules::textid() const
{
	return "diagnosis_rules";
}


PluginWidget* DiagnosisRules::widget() const
{
	return new MainDiagnosisWidget;
}


void DiagnosisRules::setInterfaces(const InterfacesPtr &interfaces)
{
	DiagnosisRules::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, DiagnosisRules)
