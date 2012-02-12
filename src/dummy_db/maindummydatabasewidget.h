#pragma once


#include "pluginwidget.h"
#include "ui_maindummydatabasewidget.h"

#include "dummydata.h"


class MainDummyDatabaseWidget : public PluginWidget, private Ui::MainDummyDatabaseWidget
{
	Q_OBJECT
public:
	explicit MainDummyDatabaseWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	DummyDataPtr m_dummyData;

private slots:
	void updateInformation();
	void createPatients();
	void createStaff();
	void createDepartments();
	void createExaminations();

};
