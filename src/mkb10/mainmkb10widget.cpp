#include "mainmkb10widget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "macros.h"
#include "mkb10treemodel.h"


MainMkb10Widget::MainMkb10Widget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	initTreeWidget();

	connect(treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
			SLOT(treeWidgetItemExpanded(QTreeWidgetItem*)));
}


void MainMkb10Widget::initTreeWidget()
{
	QSqlQuery q("SELECT id, description FROM mkb10 WHERE parentId IS NULL ORDER BY id");
	checkQuery(q);

	QList<QTreeWidgetItem*> topLevelItems;
	while(q.next())
		topLevelItems << createTreeWidgetItem(q.record());

	treeWidget->addTopLevelItems(topLevelItems);
}


void MainMkb10Widget::treeWidgetItemExpanded(QTreeWidgetItem *item)
{
	if(itemHasDummyChildItem(item))
	{
		removeDummyChildItem(item);

		QSqlQuery q;
		q.prepare("SELECT id, description FROM mkb10 WHERE parentId = :parentId ORDER BY id");
		q.bindValue(":parentId", item->data(0, Qt::UserRole));
		q.exec();
		checkQuery(q);

		QList<QTreeWidgetItem*> children;
		while(q.next())
			children << createTreeWidgetItem(q.record());

		item->addChildren(children);
	}
}



QTreeWidgetItem* MainMkb10Widget::createTreeWidgetItem(const QSqlRecord& rec)
{
	QTreeWidgetItem* item = new QTreeWidgetItem;

	const int id = rec.value("id").toInt();
	item->setData(0, Qt::UserRole, id);
	item->setText(0, rec.value("description").toString());

	if(itemHasChildren(id))
		addDummyChildItem(item);

	return item;
}


void MainMkb10Widget::addDummyChildItem(QTreeWidgetItem* item)
{
	item->addChild(new QTreeWidgetItem);
}


void MainMkb10Widget::removeDummyChildItem(QTreeWidgetItem *item)
{
	Q_ASSERT(itemHasDummyChildItem(item));
	item->removeChild(item->child(0));
}


bool MainMkb10Widget::itemHasDummyChildItem(const QTreeWidgetItem* item)
{
	return (item->childCount() == 1 && item->child(0)->data(0, Qt::UserRole).isNull());
}


bool MainMkb10Widget::itemHasChildren(const int id)
{
	QSqlQuery q;
	q.prepare("SELECT COUNT(*) FROM mkb10 WHERE parentId = :id");
	q.addBindValue(id);
	q.exec();
	checkQuery(q);
	q.first();

	return (q.value(0).toInt() > 0);
}
