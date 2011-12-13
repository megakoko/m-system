#pragma once

#include "pluginwidget.h"
#include "ui_maindepartmentswidget.h"


class QSqlQueryModel;

class MainDepartmentsWidget : public PluginWidget, private Ui::MainDepartmentsWidget
{
	Q_OBJECT

public:
	explicit MainDepartmentsWidget(QWidget *parent = 0);



private:
	void init();
	void initConnections();

	QSqlQueryModel* m_staffModel;
	QSqlQueryModel* m_departmentModel;

	bool selectedStaffCount() const;
	int selectedStaffId() const;

	bool selectedDepartmentCount() const;
	int selectedDepartmentId() const;

private slots:
	void editHealthFacility();


	void updateStaffList();

	void staffSelectionChanged();

	void addStaff();
	void editStaff();
	void deleteStaff();


	void updateDepartmentList();

	void departmentSelectionChanged();

	void addDepartment();
	void editDepartment();
	void deleteDepartment();

};
