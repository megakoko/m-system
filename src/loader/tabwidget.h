

#pragma once

#include <QTabWidget>

class PluginWidget;


class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	explicit TabWidget(QWidget *parent = 0);
	void closeAllTabs();

protected:
	void tabInserted(int index);

public slots:
	void closeTab(int index);
	void closeTab();

	void setTabLabel(const QString& text);

	void addWidget(PluginWidget* widget, const QString& caption);

};
