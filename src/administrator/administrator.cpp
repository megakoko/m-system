#include "administrator.h"


#include <QString>
#include <QtPlugin>

#include "adminwidget.h"


Administrator::~Administrator()
{

}


QString Administrator::textid() const
{
	return "admin";
}

PluginWidget* Administrator::widget() const
{
	return new AdminWidget();
}

Q_EXPORT_PLUGIN2(PluginInterface, Administrator)
