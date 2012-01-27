#pragma once

#include "pluginwidget.h"
#include "ui_mainmkb10widget.h"


class QSqlRecord;
class QSqlQueryModel;

class MainMkb10Widget : public PluginWidget, private Ui::MainMkb10Widget
{
	Q_OBJECT

public:
	explicit MainMkb10Widget(QWidget *parent = 0);

private:
	static QTreeWidgetItem* createItem(const QSqlRecord& rec);
	static QList<QTreeWidgetItem*> createItems(const QVariant& parentId = QVariant());

	void initTreeWidget();

	static const QString filterText;
	QSqlQueryModel* m_filterModel;


private slots:
	void treeWidgetItemExpanded(QTreeWidgetItem* item);

	void filterDeseases();
};
