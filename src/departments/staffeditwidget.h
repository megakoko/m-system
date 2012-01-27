#pragma once

#include "saveablepluginwidget.h"
#include "ui_staffeditwidget.h"

class StaffEditWidget : public SaveablePluginWidget, private Ui::StaffEditWidget
{
    Q_OBJECT

public:
	explicit StaffEditWidget(const int staffId, QWidget *parent = 0);

	QString staffName() const;

	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();
	void initConnections();

	const int m_staffId;

private slots:
	void nameChanged();

};
