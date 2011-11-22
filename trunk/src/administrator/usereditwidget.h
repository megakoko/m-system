#pragma once


#include "../loader/pluginwidget.h"
#include "ui_usereditwidget.h"


class UserEditWidget : public PluginWidget, private Ui::UserEditWidget
{
    Q_OBJECT

public:
	explicit UserEditWidget(const int userId, QWidget *parent = 0);

	static bool userIsAdmin(int userid);
	static int numberOfAdminUsers();

private:
	void init();
	void initConnections();

	QSet<QString> databaseTextids() const;
	QSet<QString> checkedTextids() const;


	QMap<QString, QCheckBox*> m_textidToCheckbox;
	QMap<QCheckBox*, QString> m_checkboxToTextid;

	const int m_userId;

private slots:
	void save();
	void isAdminToggled(bool);

};

