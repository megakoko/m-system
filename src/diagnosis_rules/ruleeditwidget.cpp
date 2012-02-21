#include "ruleeditwidget.h"

#include <QToolButton>
#include <QComboBox>
#include <QDebug>
#include "ruleitemeditdialog.h"


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


RuleEditWidget::RuleEditWidget(QWidget* parent)
	: SaveablePluginWidget(parent)
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
}


void RuleEditWidget::initConnections()
{
	// TODO
	connect(m_addRuleItem, SIGNAL(clicked()), SLOT(addRuleItem()));
	connect(m_removeRuleItem, SIGNAL(clicked()), SLOT(removeRuleItem()));
}


bool RuleEditWidget::canSave(QString &errorDescription) const
{
	Q_UNUSED(errorDescription);
	return false;
}


void RuleEditWidget::save()
{

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


void RuleEditWidget::removeRuleItem()
{
	// TODO
}
