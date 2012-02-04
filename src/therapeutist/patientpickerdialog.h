#pragma once

#include "ui_patientpickerdialog.h"

class QSqlQueryModel;
class SortFilterProxyModel;


class PatientPickerDialog : public QDialog, private Ui::PatientPickerDialog
{
	Q_OBJECT
public:
	explicit PatientPickerDialog(QWidget *parent = 0);

	int selectedPatientId() const;

	static const int InvalidId = 0;

private:
	void init();
	void initConnections();

	QString patientListQuery() const;


	QSqlQueryModel* m_queryModel;
	SortFilterProxyModel* m_sortModel;

private slots:
	void updatePatientsList();
	void selectionChanged();

};