#pragma once

#include "ui_patientpickerdialog.h"

class QSqlQueryModel;
class YoSortFilterProxyModel;


class PatientPickerDialog : public QDialog, private Ui::PatientPickerDialog
{
	Q_OBJECT
public:
	explicit PatientPickerDialog(QWidget *parent = 0);

	int selectedPatientId() const;

private:
	void init();
	void initConnections();

	QString patientListQuery() const;


	QSqlQueryModel* m_queryModel;
	YoSortFilterProxyModel* m_sortModel;

private slots:
	void updatePatientsList();
	void selectionChanged();

};
