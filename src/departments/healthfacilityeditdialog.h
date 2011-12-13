#pragma once

#include "ui_healthfacilityeditdialog.h"

class HealthFacilityEditDialog : public QDialog, private Ui::HealthFacilityEditDialog
{
    Q_OBJECT

public:
    explicit HealthFacilityEditDialog(QWidget *parent = 0);

	static QString toString();

private:
	void init();

private slots:
	void save();

};
