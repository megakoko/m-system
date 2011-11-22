
#pragma once

#include "ui_settingsdialog.h"


class AbstractSettingsPage;

class SettingsDialog : public QDialog, private Ui::SettingsDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

private:
	QList<AbstractSettingsPage*> m_pages;
};
