
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QLabel>


#include "pluginwidget.h"
#include "homepage.h"
#include "settingsdialog.h"
#include "logindialog.h"
#include "aboutdialog.h"
#include "plugininterface.h"
#include "macros.h"


QList<QPluginLoader*> MainWindow::m_plugins = QList<QPluginLoader*>();


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_settingsDialog(new SettingsDialog(this))
	, m_homePage(new HomePage(this))
	, m_aboutDialog(new AboutDialog(this))
{
	setupUi(this);

#ifndef QT_NO_DEBUG
	setWindowTitle(windowTitle() + QString::fromUtf8(" [Отладочная версия]"));
#endif

	initToolBar();
	initConnections();


	addHomeTab();
}


void MainWindow::initToolBar()
{
	m_connectionAction = m_toolbar->addAction(QString::fromUtf8("Войти в систему"),
											  this, SLOT(logIn()));
	m_toolbar->addAction(QString::fromUtf8("Настройки"),
						 m_settingsDialog, SLOT(exec()));

	// Небольшой хак для того, чтоб пункт "О программе" был выровнян по правому краю.
	QWidget* spacer = new QWidget(m_toolbar);
	spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	m_toolbar->addWidget(spacer);


	m_toolbar->addAction(QString::fromUtf8("О программе"), m_aboutDialog, SLOT(exec()));
}


void MainWindow::initConnections()
{
	connect(m_homePage, SIGNAL(buttonClicked(QString)), SLOT(launchPlugin(QString)));
}



QMap<QString, QString> MainWindow::pluginTextidToDescription()
{
	QMap<QString, QString> result;
	foreach(QPluginLoader* loader, m_plugins)
	{
		PluginInterface* plugin = qobject_cast<PluginInterface*>(loader->instance());
		if(plugin != NULL)
			result[plugin->textid()] = plugin->name();
	}
	return result;
}


void MainWindow::addHomeTab()
{
	m_tabWidget->addTab(m_homePage, QString::fromUtf8("Домашняя страница"));
}


void MainWindow::logIn()
{
	LoginDialog d;
	if(d.exec() == QDialog::Accepted)
	{
		m_userId = d.loggedUserId();

		m_connectionAction->setText(QString::fromUtf8("Выйти из системы"));
		disconnect(m_connectionAction, SIGNAL(triggered()), this, SLOT(logIn()));
		connect(m_connectionAction, SIGNAL(triggered()), SLOT(logOut()));

		loadPlugins();
	}
	else
		m_userId = -1;
}


void MainWindow::logOut()
{
	m_connectionAction->setText(QString::fromUtf8("Войти в систему"));
	disconnect(m_connectionAction, SIGNAL(triggered()), this, SLOT(logOut()));
	connect(m_connectionAction, SIGNAL(triggered()), SLOT(logIn()));

	m_homePage->clearButtons();
	m_tabWidget->closeAllTabs();

	unloadPlugins();
}


void MainWindow::launchPlugin(const QString &textid)
{
	foreach(QPluginLoader* loader, m_plugins)
	{
		PluginInterface* plugin = qobject_cast<PluginInterface*>(loader->instance());
		if(plugin != NULL && plugin->textid() == textid)
			m_tabWidget->addWidget(plugin->widget(), plugin->name());
	}
}


void MainWindow::loadPlugins()
{
	QDir dir(QApplication::applicationDirPath());
	if (!dir.cd("plugins"))
	{
		qWarning() << "plugins directory does not exist";
		return;
	}

	foreach (QString strFileName, dir.entryList(QDir::Files)) {
		QPluginLoader* loader = new QPluginLoader(dir.absoluteFilePath(strFileName), this);
		if(!processPlugin(loader))
			delete loader;
	}
}


void MainWindow::unloadPlugins()
{
	foreach(QPluginLoader* loader, m_plugins)
		loader->unload();

	qDeleteAll(m_plugins);
	m_plugins.clear();
}


bool MainWindow::processPlugin(QPluginLoader *obj)
{
	bool loadedOkay = false;

	obj->load();	// Явный вызов метода.
	PluginInterface* plugin = qobject_cast<PluginInterface*>(obj->instance());
	if(plugin != NULL && userHaveAccessToPlugin(plugin->textid()))
	{
		loadedOkay = true;

		m_plugins << obj;
		m_homePage->addButton(plugin->name(), plugin->textid());
	}

	if(!loadedOkay)
		obj->unload();

	return loadedOkay;
}


bool MainWindow::userHaveAccessToPlugin(const QString &textid) const
{
	Q_ASSERT(m_userId >= 0);

	bool result = false;
	QSqlQuery q;

	q.prepare("SELECT is_admin FROM MUser WHERE id = :userid");
	q.addBindValue(m_userId);
	q.exec();
	checkQuery(q);
	q.first();

	if(q.value(0).toBool())
		result = true;
	else
	{
		q.prepare(" SELECT u.id FROM MUser u "
				  " LEFT JOIN UserPluginAccess upl ON u.id = upl.userid "
				  " LEFT JOIN Plugins p ON p.id = upl.pluginid "
				  " WHERE p.textid = :textid AND u.id = :userid ");
		q.bindValue(":textid", textid);
		q.bindValue(":userid", m_userId);
		q.exec();
		checkQuery(q);

		result = q.first();
	}

	return result;
}
