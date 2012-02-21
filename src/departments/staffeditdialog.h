#pragma once

#include "ui_staffeditdialog.h"

class StaffEditDialog : public QDialog, private Ui::StaffEditWidget
{
    Q_OBJECT

public:
	explicit StaffEditDialog(const int staffId, QWidget *parent = 0);

private:
	void init();
	void initConnections();

	const int m_staffId;

private slots:
	void checkFields();
	void save();

	void nameChanged();

};
