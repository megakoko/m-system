#include "administrator.h"


#include <QString>
#include <QtPlugin>

#include "mainadminwidget.h"


Administrator::~Administrator()
{

}


QString Administrator::textid() const
{
	return "admin";
}

PluginWidget* Administrator::widget() const
{
	return new MainAdminWidget();
}

Q_EXPORT_PLUGIN2(PluginInterface, Administrator)
