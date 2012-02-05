#include "dummydatabase.h"
#include <QtPlugin>
#include "maindummydatabasewidget.h"

InterfacesPtr DummyDatabase::interfaces;

DummyDatabase::~DummyDatabase()
{

}


QString DummyDatabase::textid() const
{
	return "dummy_db";
}


PluginWidget* DummyDatabase::widget() const
{
	return new MainDummyDatabaseWidget();
}

void DummyDatabase::setInterfaces(const InterfacesPtr &interfaces)
{
	DummyDatabase::interfaces = interfaces;
}

Q_EXPORT_PLUGIN2(PluginInterface, DummyDatabase)
