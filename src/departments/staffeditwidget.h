#pragma once

#include "saveablepluginwidget.h"
#include "ui_staffeditwidget.h"

class StaffEditWidget : public SaveablePluginWidget, private Ui::StaffEditWidget
{
    Q_OBJECT

public:
	explicit StaffEditWidget(const int staffId, QWidget *parent = 0);


	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();
	const int m_staffId;

};
