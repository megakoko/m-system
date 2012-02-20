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
}


void RuleEditWidget::removeRuleItem()
{
	// TODO
}
