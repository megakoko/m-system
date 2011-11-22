

#pragma once
#include "ui_mainwindow.h"

class QPluginLoader;
class QPushButton;
class PluginInterface;
class LoginDialog;
class SettingsDialog;
class HomePage;
class AboutDialog;


class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	/// Constructs the widget with \a parent as parent.
	explicit MainWindow(QWidget *parent = 0);

	static QMap<QString, QString> pluginTextidToDescription();

private:

	void initToolBar();
	void initConnections();

	void addHomeTab();

	void loadPlugins();
	void unloadPlugins();
	/// \returns true if plugin was loaded successfully.
	bool processPlugin(QPluginLoader* obj);

	/// Holds ID of the user.
	int m_userId;

	SettingsDialog* m_settingsDialog;
	HomePage* m_homePage;
	AboutDialog* m_aboutDialog;

	bool userHaveAccessToPlugin(const QString& textid) const;


//	QMap<QString, PluginInterface*> m_textidToInterface;
//	static QMap<QString, QString> m_textidToDescription;

	QAction* m_connectionAction;

	static QList<QPluginLoader*> m_plugins;

private slots:
	void logIn();
	void logOut();
	void launchPlugin(const QString& textid);

};
