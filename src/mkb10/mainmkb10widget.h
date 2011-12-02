#pragma once

#include "pluginwidget.h"
#include "ui_mainmkb10widget.h"


class QSqlRecord;

class MainMkb10Widget : public PluginWidget, private Ui::MainMkb10Widget
{
	Q_OBJECT

public:
	explicit MainMkb10Widget(QWidget *parent = 0);

private:
	static QTreeWidgetItem* createTreeWidgetItem(const QSqlRecord& rec);

	static void addDummyChildItem(QTreeWidgetItem* item);
	static void removeDummyChildItem(QTreeWidgetItem* item);
	static bool itemHasDummyChildItem(const QTreeWidgetItem* item);

	static bool itemHasChildren(const int id);


private slots:
	void initTreeWidget();
	void treeWidgetItemExpanded(QTreeWidgetItem* item);
};
