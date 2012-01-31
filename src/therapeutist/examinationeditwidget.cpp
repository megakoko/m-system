#include "examinationeditwidget.h"

ExaminationEditWidget::ExaminationEditWidget(QWidget *parent)
	: SaveablePluginWidget(parent)
{
	setupUi(this);
}


bool ExaminationEditWidget::canSave(QString &errorDescription) const
{
	Q_UNUSED(errorDescription);
	// todo
	return false;
}


void ExaminationEditWidget::save()
{
	// todo
}
