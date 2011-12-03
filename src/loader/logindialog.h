

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


	/// Устанавливает соединение с БД.
	/// \returns	true после удачного соединения, в противном случае false.
	bool connectToDatabase();

	static const int NO_USER_ID;

	int m_userId;

	QSettings m_settings;

	/// \returns соль, хранящаяся в БД для пользователя с логином \a login.
	QByteArray salt(const QString& login) const;


	bool databaseIsInitialized() const;
	void initializeDatabase();

private slots:
	/// Пытается войти в систему.
	/// Вызывает accepted() в случае успеха.
	void tryToLogin();


	void enableOkButton();
};
