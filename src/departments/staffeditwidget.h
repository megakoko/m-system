#pragma once

#include "ui_staffeditwidget.h"

class StaffEditWidget : public QDialog, private Ui::StaffEditWidget
{
    Q_OBJECT

public:
	explicit StaffEditWidget(const int staffId, QWidget *parent = 0);

private:
	void init();
	void initConnections();

	const int m_staffId;

private slots:
	void checkFields();
	void save();

	void nameChanged();

};
