#include "diagnosisrules.h"

#include <QtPlugin>



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
	return NULL;
}


void DiagnosisRules::setInterfaces(const InterfacesPtr &interfaces)
{
	DiagnosisRules::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, DiagnosisRules)
