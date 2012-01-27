#pragma once

#include "ui_mainuserswidget.h"
#include "pluginwidget.h"


class QSqlQueryModel;

class MainUsersWidget : public PluginWidget, private Ui::MainUsersWidget
{
	Q_OBJECT

public:
	explicit MainUsersWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	bool singleUserSelected() const;

	/// \returns	ID выбранного пользователя или -1 если выбрано более 1 пользователей
	///				или 0 пользователей.
	int selectedUserId() const;

	QString selectedUserName() const;


	QSqlQueryModel* m_model;

private slots:
	void userSelectionChanged();

	void updateUserList();

	void createUser();
	void editUser();
	void deleteUser();

};
