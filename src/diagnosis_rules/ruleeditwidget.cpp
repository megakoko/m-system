#include "ruleeditwidget.h"

#include <QToolButton>
#include <QComboBox>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>

#include "ruleitemeditdialog.h"
#include "macros.h"
#include "diagnosisrules.h"


QString RuleEditWidget::formatProbability(const double &probability)
{
	return QString::number(probability, 'f', 2);
}


namespace Columns {
	enum {
		symptom,
		// itemOperator,
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
	// TODO
	static const QChar notSign(0x00AC);	// Знак, обозначающий "не".

	m_itemsTable->setColumnCount(Columns::count);
	m_itemsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_itemsTable->setHorizontalHeaderLabels(QStringList()
				<< "Симптом" << "Значение"
				<< "Вероятность P(S|D)" << QString("Вероятность P(S|%1D)").arg(notSign));

	ruleItemSelectionChanged();
}


void RuleEditWidget::initConnections()
{
	// TODO
	connect(m_addRuleItem, SIGNAL(clicked()), SLOT(addRuleItem()));
	connect(m_editRuleItem, SIGNAL(clicked()), SLOT(editRuleItem()));
	connect(m_removeRuleItem, SIGNAL(clicked()), SLOT(removeRuleItem()));

	connect(m_itemsTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(ruleItemSelectionChanged()));
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
	q.bindValue(":diseaseProbability", m_diseaseProbability->value());

	q.exec();
	checkQuery(q);

	if(m_ruleId == InvalidId)
		m_ruleId = DiagnosisRules::interfaces->db->lastInsertedId(&q).toInt();

	for(int i = 0; i < m_ruleItems.size(); ++i)
		m_ruleItems[i].save(m_ruleId);

	for(int i = 0; i < m_removedRuleItems.size(); ++i)
		m_removedRuleItems[i].deleteRuleItem();
}


void RuleEditWidget::addRuleItem()
{
	// TODO
	RuleItemEditDialog d(RuleItem(), this);
	if(d.exec() == QDialog::Accepted)
	{
		RuleItem ruleItem = d.ruleItem();
		m_ruleItems << ruleItem;

		const int row = m_itemsTable->rowCount();
		m_itemsTable->insertRow(row);

		QTableWidgetItem* item;

		item = new QTableWidgetItem(ruleItem.symptomName());
		m_itemsTable->setItem(row, Columns::symptom, item);

		item = new QTableWidgetItem(ruleItem.value());
		m_itemsTable->setItem(row, Columns::value, item);

		item = new QTableWidgetItem(formatProbability(ruleItem.probabilityWithDisease()));
		m_itemsTable->setItem(row, Columns::probabilityWithDisease, item);

		item = new QTableWidgetItem(formatProbability(ruleItem.probabilityWithoutDisease()));
		m_itemsTable->setItem(row, Columns::probabilityWithoutDisease, item);
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
	// TODO
}


void RuleEditWidget::ruleItemSelectionChanged()
{
	const bool enableButtons =
			m_itemsTable->selectionModel()->hasSelection() &&
			m_itemsTable->selectionModel()->selectedRows(0).size() == 1;

	m_editRuleItem->setEnabled(enableButtons);
	m_removeRuleItem->setEnabled(enableButtons);
}
