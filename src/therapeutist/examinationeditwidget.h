#pragma once

#include "saveablepluginwidget.h"
#include "ui_examinationeditwidget.h"

class ExaminationEditWidget : public SaveablePluginWidget, private Ui::ExaminationEditWidget
{
	Q_OBJECT

public:
	explicit ExaminationEditWidget(QWidget *parent = 0);

	bool canSave(QString &errorDescription) const;
	void save();
};
