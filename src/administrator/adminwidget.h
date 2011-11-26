#pragma once

#include "ui_adminwidget.h"
#include "pluginwidget.h"


class QSqlQueryModel;

class AdminWidget : public PluginWidget, private Ui::AdminWidget
{
	Q_OBJECT

public:
	explicit AdminWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	bool singleUserSelected() const;

	/// \returns id of selected user or -1 if there	are more than 1 or no user selected.
	int selectedUserId() const;

	QString selectedUserName() const;

	QString generateLogin() const;

	void updateUserList();


	QSqlQueryModel* m_model;

private slots:
	void userSelectionChanged();

	void createUser();
	void editUser();
	void deleteUser();

};
