#pragma once

#include "../loader/pluginwidget.h"
#include "ui_mainpatientswidget.h"


class QSqlQueryModel;

class MainPatientsWidget : public PluginWidget, private Ui::PatientsWidget
{
	Q_OBJECT

public:
	explicit MainPatientsWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	void updatePatientsList();

	bool onePatientSelected() const;
	int selectedPatientId() const;

	QSqlQueryModel* m_model;

private slots:
	void addPatient();
	void editPatient();
	void deletePatient();

	void selectionChanged();

};
