

#pragma once

#include <QTabWidget>

class PluginWidget;


class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	explicit TabWidget(QWidget *parent = 0);
	void closeAllTabs();

public slots:
	void closeTab(int index);
	void closeTab();

	void setTabLabel(const QString& text);

	void addWidget(PluginWidget* widget, const QString& caption);

protected:
	void tabInserted(int index);

private:
	bool userWantsToSave();

};
