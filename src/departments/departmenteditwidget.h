#pragma once

#include "saveablepluginwidget.h"
#include "ui_departmenteditwidget.h"

#include "staffposition.h"

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

	QList<StaffPosition> m_staffPosition;
	QList<StaffPosition> m_staffPositionMarkedForDeletion;

	void init();

private slots:
	void nameChanged();

	void staffPositionSelectionChanged();

	void addStaffPosition();
	void editStaffPosition();
	void deleteStaffPosition();

};
