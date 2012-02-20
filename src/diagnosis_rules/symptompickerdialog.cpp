#include "symptompickerdialog.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

#include "macros.h"


static const int PAGE_TREE = 0;
static const int PAGE_FILTER = 1;


const QString SymptomPickerDialog::filterText =
	" SELECT id, typeid, label "
	" FROM uielement "
	" WHERE label ILIKE '%%1%' AND typeid <> 'container'";


SymptomPickerDialog::SymptomPickerDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	init();
	initConnections();
}


void SymptomPickerDialog::init()
{
	initTreeWidget();
	selectionChanged();

	m_stackedWidget->setCurrentIndex(PAGE_TREE);

	m_filterModel = new QSqlQueryModel(this);
	m_tableView->setModel(m_filterModel);
	m_tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	m_searchWidget->setLabelText("Поиск по описанию:");
}


void SymptomPickerDialog::initConnections()
{
	connect(m_treeWidget,
			SIGNAL(itemExpanded(QTreeWidgetItem*)),
			SLOT(treeWidgetItemExpanded(QTreeWidgetItem*)));
	connect(m_searchWidget, SIGNAL(searchPressed()), SLOT(filterSymptoms()));


	connect(m_treeWidget->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));
	connect(m_tableView->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));


	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));
}


void SymptomPickerDialog::initTreeWidget()
{
	QList<QTreeWidgetItem*> topLevelItems = createItems();
	foreach(QTreeWidgetItem* item, topLevelItems)
	{
		const int& id = item->data(0, Qt::UserRole).toInt();
		item->addChildren(createItems(id));
	}

	m_treeWidget->addTopLevelItems(topLevelItems);
}


void SymptomPickerDialog::filterSymptoms()
{
	// Средство против SQL-инъекции.
	const QString& filter = m_searchWidget->enteredText().simplified().remove("'");

	if(filter.isEmpty())
		m_stackedWidget->setCurrentIndex(PAGE_TREE);
	else
	{
		m_stackedWidget->setCurrentIndex(PAGE_FILTER);
		m_filterModel->setQuery(filterText.arg(filter));
		checkQuery(m_filterModel->query());

		m_tableView->setColumnHidden(0, true);
		m_tableView->setColumnHidden(1, true);
	}

	selectionChanged();
}



QTreeWidgetItem* SymptomPickerDialog::createItem(const QSqlRecord& rec)
{
	QTreeWidgetItem* item = new QTreeWidgetItem;

	const QVariant& id = rec.value("id");
	const QString& description = rec.value("label").toString();

	item->setData(0, Qt::UserRole, id);
	item->setData(0, Qt::UserRole+1, rec.value("typeid"));
	item->setText(0, description);
	item->setToolTip(0, description);

	return item;
}


QList<QTreeWidgetItem*> SymptomPickerDialog::createItems(const QVariant& parentId
														 /* = QVariant() */)
{
	QSqlQuery q;
	if(parentId.isNull())
	{
		q.prepare(" SELECT id, label, typeid FROM uielement "
				  " WHERE parentId = 'main' AND typeid = 'container' "
				  " ORDER BY id ");
	}
	else
	{
		q.prepare(" SELECT id, label, typeid FROM uielement "
				  " WHERE parentId = (SELECT textid FROM UiElement where id = :id) "
				  " ORDER by id");
		q.addBindValue(parentId);
	}

	q.exec();
	checkQuery(q);

	QList<QTreeWidgetItem*> result;
	while(q.next())
		result << createItem(q.record());

	return result;
}


void SymptomPickerDialog::treeWidgetItemExpanded(QTreeWidgetItem *item)
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


void SymptomPickerDialog::selectionChanged()
{
	static const QVariant container = "container";


	bool enableButton = false;
	if(m_stackedWidget->currentIndex() == PAGE_TREE)
	{
		QList<QTreeWidgetItem*> items = m_treeWidget->selectedItems();

		if(items.size() == 1 && items.first()->data(0, Qt::UserRole+1) != container)
			enableButton = true;
	}
	else if(m_stackedWidget->currentIndex() == PAGE_FILTER)
	{
		QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows(0);

		if(selectedIndexes.size() == 1)
		{
			const int row = selectedIndexes.first().row();
			if(m_filterModel->record(row).value("typeid") != container)
				enableButton = true;
		}
	}

	m_ok->setEnabled(enableButton);
}


int SymptomPickerDialog::selectedSymptom() const
{
	int id = 0;	// TODO

	if(m_stackedWidget->currentIndex() == PAGE_TREE)
	{
		QList<QTreeWidgetItem*> items = m_treeWidget->selectedItems();
		if(!items.isEmpty())
			id = items.first()->data(0, Qt::UserRole).toInt();
	}
	else if(m_stackedWidget->currentIndex() == PAGE_FILTER)
	{
		QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows(0);

		if(!selectedIndexes.isEmpty())
		{
			const int row = selectedIndexes.first().row();
			id = m_filterModel->record(row).value("id").toInt();
		}
	}

	return id;
}
