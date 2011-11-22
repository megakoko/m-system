

#pragma once

#include <QSettings>
#include "ui_logindialog.h"


class LoginDialog : public QDialog, private Ui::LoginDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = 0);

	int loggedUserId() const;

private:
	void initConnections();

	void showMessage(const QString& message);


	/// Connects to the database.
	/// \returns true on success, otherwise false.
	bool connectToDatabase();

	static const int NO_USER_ID;

	int m_userId;

	QSettings m_settings;

	/// \returns salt stored in the database for the user with given \a login.
	QByteArray salt(const QString& login) const;

private slots:
	/// Tries to log in into the system.
	/// Calls accepted() on success.
	void tryToLogin();


	void enableOkButton();
};
