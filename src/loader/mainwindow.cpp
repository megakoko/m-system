
#include "mainwindow.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>


#include "pluginwidget.h"
#include "homepage.h"
#include "settingsdialog.h"
#include "logindialog.h"
#include "aboutdialog.h"
#include "plugininterface.h"
#include "macros.h"
#include "cryptopp/wrapper.h"
#include "database.h"
#include "user.h"
#include "demo.h"


QList<QPluginLoader*> MainWindow::m_plugins = QList<QPluginLoader*>();
InterfacesPtr MainWindow::interfaces = InterfacesPtr(new Interfaces(new CryptoppWrapper,
																	new Database,
																	new User,
																	new Demo));


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_settingsDialog(new SettingsDialog(this))
	, m_homePage(new HomePage(this))
{
	setupUi(this);

	QSettings settings;
	restoreGeometry(settings.value("windowGeometry").toByteArray());

#ifndef QT_NO_DEBUG
	setWindowTitle(windowTitle() + QString::fromUtf8(" [Отладочная версия]"));
#endif

	if(interfaces->demo->isDemoVersion())
		setWindowTitle(windowTitle() + " [Демонстрационная версия]");

	initToolBar();
	initConnections();


	addHomeTab();
}


MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("windowGeometry", saveGeometry());
}


void MainWindow::initToolBar()
{
	m_loginAction = m_toolbar->addAction(QIcon(":/login.png"), "Войти в систему",
										 this, SLOT(logIn()));
	m_logoutAction = m_toolbar->addAction(QIcon(":/logout.png"), "Выйти из системы",
										  this, SLOT(logOut()));
	m_saveAndCloseAction = m_toolbar->addAction(QIcon(":/save.png"), "Сохранить и закрыть",
												m_tabWidget, SLOT(closeCurrentTab()));

	m_logoutAction->setVisible(false);


	// Небольшой хак для того, чтоб пункт "О программе" был выровнян по правому краю.
	QWidget* spacer = new QWidget(m_toolbar);
	spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	m_toolbar->addWidget(spacer);


	m_toolbar->addAction(QIcon(":/settings.png"), "Настройки",
						 m_settingsDialog, SLOT(exec()));
	m_toolbar->addAction(QIcon(":/about.png"), "О программе",
						 this, SLOT(showAboutDialog()));
}


void MainWindow::initConnections()
{
	connect(m_homePage, SIGNAL(buttonClicked(QString)), SLOT(launchPlugin(QString)));
	connect(m_tabWidget, SIGNAL(currentWidgetChanged(bool)), SLOT(currentTabChanged(bool)));
}


void MainWindow::addHomeTab()
{
	m_tabWidget->addHomeTab(m_homePage);
}


void MainWindow::logIn()
{
	LoginDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_userId = d.loggedUserId();
		loadPlugins();

		m_loginAction->setVisible(false);
		m_logoutAction->setVisible(true);
	}
	else
		m_userId = -1;
}


void MainWindow::logOut()
{
	const int answer = QMessageBox::question(this, "Выход из системы",
											 "Вы действительно хотите выйти из системы? "
											 "Все несохраненные данные будут утеряны.",
											 QMessageBox::Yes | QMessageBox::No);


	if(answer == QMessageBox::Yes)
	{
		m_homePage->clearButtons();
		m_tabWidget->closeAllTabs();

		LoginDialog::disconnectFromDatabase();

		unloadPlugins();

		m_loginAction->setVisible(true);
		m_logoutAction->setVisible(false);
	}
}


void MainWindow::launchPlugin(const QString &textid)
{
	foreach(QPluginLoader* loader, m_plugins)
	{
		PluginInterface* plugin = qobject_cast<PluginInterface*>(loader->instance());
		if(plugin != NULL && plugin->textid() == textid)
			m_tabWidget->addWidget(plugin->widget(), pluginName(textid), textid);
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
		{
			qDebug() << "failed to load" << strFileName;
			delete loader;
		}
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
		plugin->setInterfaces(interfaces);

		loadedOkay = true;

		m_plugins << obj;
		m_homePage->addButton(pluginName(plugin->textid()), plugin->textid());
	}

	if(!loadedOkay)
		obj->unload();

	return loadedOkay;
}


bool MainWindow::userHaveAccessToPlugin(const QString &textid) const
{
	Q_ASSERT(m_userId >= 0);


// В релизе модуль plugin_example не должен загружаться.
#ifdef QT_NO_DEBUG
	if(textid == "plugin_example")
        return false;
#endif

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
				  " LEFT JOIN Plugin p ON p.id = upl.pluginid "
				  " WHERE p.textid = :textid AND u.id = :userid ");
		q.bindValue(":textid", textid);
		q.bindValue(":userid", m_userId);
		q.exec();
		checkQuery(q);

		result = q.first();
	}

	return result;
}


QString MainWindow::pluginName(const QString &textid) const
{
	QSqlQuery q;
	q.prepare("SELECT name FROM Plugin WHERE textid = :textid");
	q.bindValue(":textid", textid);
	q.exec();
	checkQuery(q);


	return q.first() ? q.value(0).toString() : QString::null;
}


void MainWindow::showAboutDialog()
{
	AboutDialog d(this);
	d.exec();
}


void MainWindow::currentTabChanged(const bool widgetIsSaveable)
{
	m_saveAndCloseAction->setVisible(widgetIsSaveable);
}
