#include "mainmkb10widget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "macros.h"


MainMkb10Widget::MainMkb10Widget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	initTreeWidget();

	connect(m_treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
			SLOT(treeWidgetItemExpanded(QTreeWidgetItem*)));
}


void MainMkb10Widget::initTreeWidget()
{
	QList<QTreeWidgetItem*> topLevelItems = createItems();
	foreach(QTreeWidgetItem* item, topLevelItems)
	{
		const int& id = item->data(0, Qt::UserRole).toInt();
		item->addChildren(createItems(id));
	}

	m_treeWidget->addTopLevelItems(topLevelItems);
}


QTreeWidgetItem* MainMkb10Widget::createItem(const QSqlRecord& rec)
{
	QTreeWidgetItem* item = new QTreeWidgetItem;

	const int id = rec.value("id").toInt();
	item->setData(0, Qt::UserRole, id);
	item->setText(0, rec.value("description").toString());

	return item;
}


QList<QTreeWidgetItem*> MainMkb10Widget::createItems(const int parentId)
{
	QSqlQuery q;
	if(parentId > 0)
	{
		q.prepare("SELECT id, description FROM mkb10 WHERE parentId = :id");
		q.addBindValue(parentId);
	}
	else
	{
		q.prepare("SELECT id, description FROM mkb10 WHERE parentId IS NULL");
	}

	q.exec();
	checkQuery(q);

	QList<QTreeWidgetItem*> result;
	while(q.next())
		result << createItem(q.record());

	return result;
}


void MainMkb10Widget::treeWidgetItemExpanded(QTreeWidgetItem *item)
{
	for(int i = 0; i < item->childCount(); ++i)
	{
		QTreeWidgetItem* child = item->child(i);
		if(child->childCount() == 0)
		{
			const int& id = child->data(0, Qt::UserRole).toInt();
			child->addChildren(createItems(id));
		}
	}
}
