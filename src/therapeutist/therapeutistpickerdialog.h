#pragma once

#include "ui_therapeutistpickerdialog.h"


class QSqlQueryModel;

class TherapeutistPickerDialog : public QDialog, private Ui::TherapeutistPickerDialog
{
	Q_OBJECT	
public:
	explicit TherapeutistPickerDialog(QWidget *parent);
	int selectedTherapeutistId() const;
	QString selectedTherapeutistName() const;

private:
	void init();
	void initConnections();

	static QString query(QString searchText = QString::null);

	QSqlQueryModel* m_model;

private slots:
	void updateTherapeutistList();
	void listSelectionChanged();

};
