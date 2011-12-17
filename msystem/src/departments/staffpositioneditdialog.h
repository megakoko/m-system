#pragma once

#include "ui_staffpositioneditdialog.h"
#include "staffposition.h"


class StaffPositionEditDialog : public QDialog, private Ui::StaffPositionEditDialog
{
    Q_OBJECT

public:
	explicit StaffPositionEditDialog(const StaffPosition& sp, QWidget *parent = 0);

	StaffPosition currentStaffPosition();

private:
	void init();
	QVariant currentStaffId() const;
	QVariant currentPositionId() const;

	StaffPosition m_staffPosition;

private slots:
	void checkCombos();

};
