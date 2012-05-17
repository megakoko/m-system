#include "mainmkb10widget.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

#include "macros.h"
#include "mkb10.h"


static const int PAGE_TREE = 0;
static const int PAGE_FILTER = 1;


QString MainMkb10Widget::filterText()
{
	const QString& LIKE = Mkb10::interfaces->db->caseInsensitiveLike();
	return	" SELECT description "
			" FROM mkb10 "
			" WHERE description " + LIKE + " '%%1%' ";
}


MainMkb10Widget::MainMkb10Widget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	initTreeWidget();

	m_stackedWidget->setCurrentIndex(PAGE_TREE);

	m_filterModel = new QSqlQueryModel(this);
	m_listView->setModel(m_filterModel);

	connect(m_treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
			SLOT(treeWidgetItemExpanded(QTreeWidgetItem*)));

	connect(m_searchWidget, SIGNAL(searchPressed()), SLOT(filterDeseases()));
	m_searchWidget->setLabelText("Поиск по описанию:");
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


void MainMkb10Widget::filterDeseases()
{
	// Средство против SQL-инъекции.
	const QString& filter = m_searchWidget->enteredText().simplified().remove("'");

	if(filter.isEmpty())
		m_stackedWidget->setCurrentIndex(PAGE_TREE);
	else
	{
		m_stackedWidget->setCurrentIndex(PAGE_FILTER);
		m_filterModel->setQuery(filterText().arg(filter));
		checkQuery(m_filterModel->query());
	}
}



QTreeWidgetItem* MainMkb10Widget::createItem(const QSqlRecord& rec)
{
	QTreeWidgetItem* item = new QTreeWidgetItem;

	const QVariant& id = rec.value("id");
	const QString& description = rec.value("description").toString();

	item->setData(0, Qt::UserRole, id);
	item->setText(0, description);
	item->setToolTip(0, description);

	return item;
}


QList<QTreeWidgetItem*> MainMkb10Widget::createItems(const QVariant& parentId
													 /* = QVariant() */)
{
	QSqlQuery q;
	if(parentId.isNull())
	{
		q.prepare("SELECT id, description FROM mkb10 WHERE parentId IS NULL");
	}
	else
	{
		q.prepare("SELECT id, description FROM mkb10 WHERE parentId = :id");
		q.addBindValue(parentId);
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
			const QVariant& id = child->data(0, Qt::UserRole);
			child->addChildren(createItems(id));
		}
	}
}
