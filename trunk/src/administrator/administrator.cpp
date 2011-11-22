#include "administrator.h"


#include <QString>
#include <QtPlugin>

#include "adminwidget.h"


Administrator::~Administrator()
{

}


QString Administrator::name() const
{
	return QString::fromUtf8("Администратор");
}


QString Administrator::textid() const
{
	return "admin";
}

PluginWidget* Administrator::widget() const
{
	return new AdminWidget();
}

QWidget* Administrator::settingsPage() const
{
	return NULL;
}

Q_EXPORT_PLUGIN2(PluginInterface, Administrator)
