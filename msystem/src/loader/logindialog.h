

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

	static QString convertToSqliteSyntax(const QString& postgresqlSyntax);
	static void executeSqlFile(const QString& filename);

	int m_userId;

	QSettings m_settings;

	/// \returns соль, хранящаяся в БД для пользователя с логином \a login.
	QString salt(const QString& login) const;


	bool databaseIsInitialized() const;

private slots:
	void initializeDatabase();

	/// Пытается войти в систему.
	/// Вызывает accepted() в случае успеха.
	void tryToLogin();


	void enableOkButton();
};
