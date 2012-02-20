#include "ruleeditwidget.h"

#include <QToolButton>
#include <QComboBox>
#include <QDebug>



namespace LayoutColumns {
	static const int symptomLabel = 0;
	static const int symptomButton = 1;
	static const int itemOperator = 2;
	static const int value = 3;
	static const int removeButton = 4;
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
}


void RuleEditWidget::initConnections()
{
	// TODO
	connect(m_addRuleItem, SIGNAL(clicked()), SLOT(addRuleItem()));
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
	RuleItem* item = new RuleItem;

	const int row = m_ruleItemsLayout->rowCount();
	m_ruleItemsLayout->addWidget(item->symptomLabel(), row, LayoutColumns::symptomLabel);
	m_ruleItemsLayout->addWidget(item->symptomButton(), row, LayoutColumns::symptomButton);
	m_ruleItemsLayout->addWidget(item->itemOperator(), row, LayoutColumns::itemOperator);
	m_ruleItemsLayout->addWidget(item->value(), row, LayoutColumns::value);
	m_ruleItemsLayout->addWidget(item->removeButton(), row, LayoutColumns::removeButton);

	connect(item, SIGNAL(removeMe()), SLOT(removeRuleItem()));

	m_ruleItems << item;
}


void RuleEditWidget::removeRuleItem()
{
	RuleItem* item = dynamic_cast<RuleItem*>(sender());

	if(item != NULL && m_ruleItems.removeOne(item))
	{
		item->symptomLabel()->hide();
		item->symptomButton()->hide();
		item->itemOperator()->hide();
		item->value()->hide();
		item->removeButton()->hide();

		m_removedRuleItems << item;
	}
}
