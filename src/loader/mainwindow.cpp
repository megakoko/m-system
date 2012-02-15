
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


QList<QPluginLoader*> MainWindow::m_plugins = QList<QPluginLoader*>();
InterfacesPtr MainWindow::interfaces = InterfacesPtr(new Interfaces(new CryptoppWrapper,
																	new Database));


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
	m_connectionAction = m_toolbar->addAction(QString::fromUtf8("Войти в систему"),
											  this, SLOT(logIn()));
	m_toolbar->addAction(QString::fromUtf8("Настройки"),
						 m_settingsDialog, SLOT(exec()));

	// Небольшой хак для того, чтоб пункт "О программе" был выровнян по правому краю.
	QWidget* spacer = new QWidget(m_toolbar);
	spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	m_toolbar->addWidget(spacer);


	m_toolbar->addAction(QString::fromUtf8("О программе"), this, SLOT(showAboutDialog()));
}


void MainWindow::initConnections()
{
	connect(m_homePage, SIGNAL(buttonClicked(QString)), SLOT(launchPlugin(QString)));
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
	const int answer = QMessageBox::question(this, "Выход из системы",
											 "Вы действительно хотите выйти из системы? "
											 "Все несохраненные данные будут утеряны.",
											 QMessageBox::Yes | QMessageBox::No);


	if(answer == QMessageBox::Yes)
	{
		m_connectionAction->setText(QString::fromUtf8("Войти в систему"));
		disconnect(m_connectionAction, SIGNAL(triggered()), this, SLOT(logOut()));
		connect(m_connectionAction, SIGNAL(triggered()), SLOT(logIn()));

		m_homePage->clearButtons();
		m_tabWidget->closeAllTabs();

		LoginDialog::disconnectFromDatabase();

		unloadPlugins();
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


// В релизе модуль test не должен загружаться.
#ifdef QT_NO_DEBUG
    if(textid == "test")
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
