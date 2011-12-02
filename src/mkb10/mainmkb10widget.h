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
	static QTreeWidgetItem* createItem(const QSqlRecord& rec);
	static QList<QTreeWidgetItem*> createItems(const int parentId = -1);

private slots:
	void initTreeWidget();
	void treeWidgetItemExpanded(QTreeWidgetItem* item);
};
