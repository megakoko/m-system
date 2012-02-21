#include "positionseditdialog.h"

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QDebug>

#include "departments.h"
#include "macros.h"

PositionsEditDialog::PositionsEditDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

	init();
}


void PositionsEditDialog::init()
{
	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(save()));
	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));

	connect(m_addPosition, SIGNAL(clicked()), SLOT(addPosition()));
	connect(m_deletePosition, SIGNAL(clicked()), SLOT(deletePosition()));

	connect(m_positionList, SIGNAL(itemSelectionChanged()), SLOT(selectionChanged()));
	connect(m_positionList, SIGNAL(itemChanged(QListWidgetItem*)),
			SLOT(itemChanged(QListWidgetItem*)));


	QSqlQuery q("SELECT id, name, textid FROM Position ORDER BY id");
	checkQuery(q);


	QListWidgetItem* item;
	while(q.next())
	{
		item = new QListWidgetItem(q.value(1).toString());
		item->setData(Qt::UserRole, q.value(0));

		// Если у записи определен textid, ее нельзя удалять.
		if(q.value(2).isNull())
			item->setFlags(item->flags() | Qt::ItemIsEditable);
		else
		{
			m_undeletablePositionIds << q.value(0).toInt();
			item->setToolTip("Эта должность является предопределенной и не может быть "
							 "удалена.");
		}

		m_positionList->addItem(item);
	}

	selectionChanged();
}


void PositionsEditDialog::save()
{
	QSqlQuery updateQuery, insertQuery;
	updateQuery.prepare("UPDATE Position SET name = :name WHERE id = :id");
	insertQuery.prepare("INSERT INTO Position(name) VALUES(:name)");


	for(int row = 0; row < m_positionList->count(); ++row)
	{
		const QListWidgetItem* item = m_positionList->item(row);
		const QVariant& id = m_positionList->item(row)->data(Qt::UserRole);

		// Обновление.
		if(!id.isNull() && id.canConvert<int>())
		{
			updateQuery.bindValue(":name", item->text());
			updateQuery.bindValue(":id", id.toInt());
			updateQuery.exec();
			checkQuery(updateQuery);
		}
		// Вставка.
		else
		{
			insertQuery.bindValue(":name", item->text());
			insertQuery.exec();
			checkQuery(insertQuery);
		}
	}

	// Удаление.
	QSqlQuery deleteQuery;
	deleteQuery.prepare("DELETE FROM DepartmentStaffPosition WHERE positionId = :id");
	foreach(const int id, m_markedForDeletionPositions)
	{
		deleteQuery.bindValue(":id", id);
		deleteQuery.exec();
		checkQuery(deleteQuery);
	}

	deleteQuery.prepare("DELETE FROM Position WHERE id = :id");
	foreach(const int id, m_markedForDeletionPositions)
	{
		deleteQuery.bindValue(":id", id);
		deleteQuery.exec();
		checkQuery(deleteQuery);
	}

}


void PositionsEditDialog::addPosition()
{
	QListWidgetItem* item = new QListWidgetItem(generatePositionName());
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	m_positionList->addItem(item);
}


void PositionsEditDialog::deletePosition()
{
	const int row = m_positionList->currentRow();
	Q_ASSERT(row >= 0);

	const QVariant& id = m_positionList->takeItem(row)->data(Qt::UserRole);
	if(!id.isNull() && id.canConvert<int>())
	{
		Q_ASSERT(!m_undeletablePositionIds.contains(id.toInt()));
		m_markedForDeletionPositions << id.toInt();
	}
}


void PositionsEditDialog::selectionChanged()
{
	QList<QListWidgetItem*> selectedItems = m_positionList->selectedItems();

	// Количество выделенных элементов = 1 и запись не является неудаляемой.
	const bool enableDeleteButton =
			selectedItems.count() == 1 &&
			!m_undeletablePositionIds.contains(selectedItems.first()->data(Qt::UserRole).toInt());

	m_deletePosition->setEnabled(enableDeleteButton);
}


QStringList PositionsEditDialog::positions()
{
	QSqlQuery q("SELECT name FROM Position ORDER BY id");
	checkQuery(q);

	QStringList list;
	while(q.next())
		list << q.value(0).toString();

	if(list.isEmpty())
		list << "<i>Список должностей пуст</i>";

	return list;
}


QString PositionsEditDialog::generatePositionName() const
{
	static const QString basename = "Новая должность";
	static const QString rxString = QString("^%1 \\(([0-9]+)\\)$").arg(basename);
	QList<QListWidgetItem*> items = m_positionList->findItems(rxString, Qt::MatchRegExp);


	QRegExp rx(rxString);
	QList<int> numbers;
	foreach(const QListWidgetItem* item, items)
	{
		if(rx.indexIn(item->text()) >= 0)
			numbers << rx.cap(1).toInt();
	}


	int possibleNumber = 0;
	while(++possibleNumber > 0)
		if(!numbers.contains(possibleNumber))
			break;


	return QString("%1 (%2)").arg(basename).arg(possibleNumber);
}


void PositionsEditDialog::itemChanged(QListWidgetItem* item)
{
	static const int maxLength = Departments::interfaces->db->
			fieldMaximumLength("Position", "name");

	if(maxLength >= 0 && item != NULL)
	{
		const QString& itemText = item->text();
		if(itemText.length() > maxLength)
			item->setText(itemText.left(maxLength));
	}
}
