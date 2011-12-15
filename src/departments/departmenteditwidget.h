#pragma once

#include "saveablepluginwidget.h"
#include "ui_departmenteditwidget.h"

class DepartmentEditWidget : public SaveablePluginWidget, private Ui::DepartmentEditWidget
{
    Q_OBJECT

public:
	explicit DepartmentEditWidget(const int departmentId, QWidget *parent = 0);

	QString departmentName() const;

	bool canSave(QString &errorDescription) const;
	void save();

private:
	const int m_departmentId;

	void init();

private slots:
	void nameChanged();

};
