#pragma once

#include "saveablepluginwidget.h"
#include "ui_examinationeditwidget.h"

class ExaminationEditWidget : public SaveablePluginWidget, private Ui::ExaminationEditWidget
{
	Q_OBJECT

public:
	explicit ExaminationEditWidget(const int examinationId, QWidget *parent = 0);

	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();


	int m_examinationId;
};
