#pragma once

#include "saveablepluginwidget.h"
#include "ui_departmeneditwidget.h"

class DepartmenEditWidget : public SaveablePluginWidget, private Ui::DepartmenEditWidget
{
    Q_OBJECT

public:
	explicit DepartmenEditWidget(const int departmentId, QWidget *parent = 0);

	bool canSave(QString &errorDescription) const;
	void save();

private:
	const int m_departmentId;

	void init();

};
