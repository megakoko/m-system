#pragma once

#include "pluginwidget.h"
#include "ui_mainpatientswidget.h"


class QSqlQueryModel;
class QSortFilterProxyModel;

class MainPatientsWidget : public PluginWidget, private Ui::PatientsWidget
{
	Q_OBJECT

public:
	explicit MainPatientsWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	QString patientListQuery() const;

	bool onePatientSelected() const;
	int selectedPatientId() const;

	QSqlQueryModel* m_queryModel;
	QSortFilterProxyModel* m_sortModel;


private slots:
	void updatePatientsList();

	void addPatient();
	void editPatient();
	void deletePatient();

	void selectionChanged();

};
