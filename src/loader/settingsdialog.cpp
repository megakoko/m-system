
#include "settingsdialog.h"

#include <QPushButton>

#include "abstractsettingspage.h"
#include "databasesettingspage.h"
#include "logsettingspage.h"


SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_pages << new DatabaseSettingsPage(this)
			<< new LogSettingsPage(this);


	foreach(AbstractSettingsPage* page, m_pages)
	{
		page->readSettings();
		m_listWidget->addItem(page->pageName());
		m_stackedWidget->addWidget(page);
	}

	m_stackedWidget->setCurrentIndex(0);
	connect(m_listWidget, SIGNAL(currentRowChanged(int)),
			m_stackedWidget, SLOT(setCurrentIndex(int)));

	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(save()));
	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(restore()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));


	m_listWidget->setMinimumWidth(200);
	m_listWidget->setMaximumWidth(200);
}


void SettingsDialog::restore()
{
	foreach(AbstractSettingsPage* page, m_pages)
		page->readSettings();
}


void SettingsDialog::save()
{
	foreach(AbstractSettingsPage* page, m_pages)
		page->saveSettings();
}
