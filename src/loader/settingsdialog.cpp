
#include "settingsdialog.h"

#include "abstractsettingspage.h"
#include "databasesettingspage.h"


SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_pages << new DatabaseSettingsPage(this);


	foreach(AbstractSettingsPage* page, m_pages)
	{
		page->readSettings();
		m_listWidget->addItem(page->pageName());
		m_stackedWidget->addWidget(page);
	}

	m_stackedWidget->setCurrentIndex(0);
	connect(m_listWidget, SIGNAL(currentRowChanged(int)),
			m_stackedWidget, SLOT(setCurrentIndex(int)));
}


SettingsDialog::~SettingsDialog()
{

	foreach(AbstractSettingsPage* page, m_pages)
	{
		page->saveSettings();
	}
}
