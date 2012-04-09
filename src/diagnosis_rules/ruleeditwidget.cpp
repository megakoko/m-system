#include "ruleeditwidget.h"

#include <QToolButton>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "ruleitemeditdialog.h"
#include "macros.h"
#include "diagnosisrules.h"


QString RuleEditWidget::formatProbability(const double &probability)
{
	// Вероятность переводим в промилле.
	return QLocale().toString(probability * 100.0, 'f', 2);
}


namespace Columns {
	enum {
		symptom,
		itemOperator,
		value,
		probabilityWithDisease,
		probabilityWithoutDisease,
		count
	};
}


RuleEditWidget::RuleEditWidget(const int ruleId, QWidget* parent)
	: SaveablePluginWidget(parent)
	, m_ruleId(ruleId)
{
	setupUi(this);

	init();
	initConnections();
}


void RuleEditWidget::init()
{
	static const QChar notSign(0x00AC);	// Знак, обозначающий "не".

	m_itemsTable->setColumnCount(Columns::count);
	m_itemsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_itemsTable->horizontalHeader()->setResizeMode(Columns::itemOperator,
													QHeaderView::ResizeToContents);
	m_itemsTable->horizontalHeader()->setResizeMode(Columns::probabilityWithDisease,
													QHeaderView::ResizeToContents);
	m_itemsTable->horizontalHeader()->setResizeMode(Columns::probabilityWithoutDisease,
													QHeaderView::ResizeToContents);
	m_itemsTable->setHorizontalHeaderLabels(QStringList()
				<< "Симптом" << "Оператор" << "Значение"
				<< QString("P(S|D), %1").arg("%")
				<< QString("P(S|%1D), %2").arg(notSign).arg("%"));

	ruleItemSelectionChanged();


	if(m_ruleId != InvalidId)
	{
		QSqlQuery q;
		q.prepare("SELECT diseaseText, diseaseProbability FROM DsRule WHERE id = ?");
		q.addBindValue(m_ruleId);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			// Вероятность [0;1] переводим в промилле.
			m_diseaseText->setText(q.value(0).toString());
			m_diseaseProbability->setValue(q.value(1).toDouble() * 1000.0);
		}


		q.prepare(" SELECT id, ruleId, uiElementId, operatorId, textValue, realValue, realValue2, enumValue, "
				  " probabilityWithDisease, probabilityWithoutDisease "
				  " FROM DsRuleItem WHERE ruleId = ? ");
		q.addBindValue(m_ruleId);
		q.exec();
		checkQuery(q);


		while(q.next())
		{
			m_ruleItems << RuleItem(q.record());
			addRuleToTable(m_ruleItems.last());
		}
	}
}


void RuleEditWidget::initConnections()
{
	connect(m_diseaseText, SIGNAL(editingFinished()), SLOT(updateTabName()));

	connect(m_addRuleItem, SIGNAL(clicked()), SLOT(addRuleItem()));
	connect(m_editRuleItem, SIGNAL(clicked()), SLOT(editRuleItem()));
	connect(m_removeRuleItem, SIGNAL(clicked()), SLOT(removeRuleItem()));

	connect(m_itemsTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(ruleItemSelectionChanged()));

	connect(m_itemsTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(editRuleItem()));
}


QString RuleEditWidget::tabName() const
{
	QString name = "Правило";
	if(!m_diseaseText->text().isEmpty())
		name += ": " + m_diseaseText->text();
	return name;
}


void RuleEditWidget::updateTabName()
{
	emit requestToSetTabLabel(tabName());
}


bool RuleEditWidget::canSave(QString &errorDescription) const
{
	if(m_diseaseText->text().isEmpty())
	{
		errorDescription = "Название болезни не заполнено";
		return false;
	}

	return true;
}


void RuleEditWidget::save()
{
	QSqlQuery q;
	if(m_ruleId == InvalidId)
	{
		q.prepare(" INSERT INTO DsRule "
				  " ( diseaseText,  diseaseProbability)  VALUES "
				  " (:diseaseText, :diseaseProbability) "
				  + DiagnosisRules::interfaces->db->returningSentence("id"));
	}
	else
	{
		q.prepare(" UPDATE DsRule SET "
				  " diseaseText = :diseaseText, "
				  " diseaseProbability = :diseaseProbability "
				  " WHERE id = :id ");
		q.bindValue(":id", m_ruleId);
	}

	q.bindValue(":diseaseText", m_diseaseText->text());
	// Промилле переводим обратно в вероятность [0;1].
	q.bindValue(":diseaseProbability", m_diseaseProbability->value() / 1000.0);

	q.exec();
	checkQuery(q);

	if(m_ruleId == InvalidId)
		m_ruleId = DiagnosisRules::interfaces->db->lastInsertedId(&q).toInt();

	for(int i = 0; i < m_ruleItems.size(); ++i)
	{
		m_ruleItems[i].setRuleid(m_ruleId);
		m_ruleItems[i].save();
	}

	for(int i = 0; i < m_removedRuleItems.size(); ++i)
		m_removedRuleItems[i].deleteRuleItem();


	emit saved();
}


void RuleEditWidget::addRuleToTable(const RuleItem &ruleItem)
{
	const int row = m_itemsTable->rowCount();
	m_itemsTable->insertRow(row);

	QTableWidgetItem* item;

	item = new QTableWidgetItem(ruleItem.symptomName());
	m_itemsTable->setItem(row, Columns::symptom, item);

	item = new QTableWidgetItem(ruleItem.operatorText());
	m_itemsTable->setItem(row, Columns::itemOperator, item);

	item = new QTableWidgetItem(ruleItem.value());
	m_itemsTable->setItem(row, Columns::value, item);

	item = new QTableWidgetItem(formatProbability(ruleItem.probabilityWithDisease()));
	m_itemsTable->setItem(row, Columns::probabilityWithDisease, item);

	item = new QTableWidgetItem(formatProbability(ruleItem.probabilityWithoutDisease()));
	m_itemsTable->setItem(row, Columns::probabilityWithoutDisease, item);
}


void RuleEditWidget::addRuleItem()
{
	RuleItemEditDialog d(RuleItem(), this);
	if(d.exec() == QDialog::Accepted)
	{
		m_ruleItems << d.ruleItem();
		addRuleToTable(m_ruleItems.last());
	}
}


void RuleEditWidget::editRuleItem()
{
	const int row = m_itemsTable->selectionModel()->selectedRows(0).first().row();

	RuleItemEditDialog d(m_ruleItems.at(row), this);
	if(d.exec() == QDialog::Accepted)
	{
		const RuleItem& ruleItem = d.ruleItem();
		m_ruleItems[row] = ruleItem;

		QTableWidgetItem* item;

		item = m_itemsTable->item(row, Columns::symptom);
		item->setText(ruleItem.symptomName());

		item = m_itemsTable->item(row, Columns::itemOperator);
		item->setText(ruleItem.operatorText());

		item = m_itemsTable->item(row, Columns::value);
		item->setText(ruleItem.value());

		item = m_itemsTable->item(row, Columns::probabilityWithDisease);
		item->setText(formatProbability(ruleItem.probabilityWithDisease()));

		item = m_itemsTable->item(row, Columns::probabilityWithoutDisease);
		item->setText(formatProbability(ruleItem.probabilityWithoutDisease()));
	}
}


void RuleEditWidget::removeRuleItem()
{
	const int answer = QMessageBox::question(this, "Удаление элемента",
											 "Вы действительно хотите удалить элемент?",
											 QMessageBox::Yes | QMessageBox::No,
											 QMessageBox::No);
	if(answer == QMessageBox::Yes)
	{
		const int row = m_itemsTable->selectionModel()->selectedRows(0).first().row();
		m_removedRuleItems << m_ruleItems.takeAt(row);
		m_itemsTable->removeRow(row);
	}
}


void RuleEditWidget::ruleItemSelectionChanged()
{
	const bool enableButtons =
			m_itemsTable->selectionModel()->hasSelection() &&
			m_itemsTable->selectionModel()->selectedRows(0).size() == 1;

	m_editRuleItem->setEnabled(enableButtons);
	m_removeRuleItem->setEnabled(enableButtons);
}
