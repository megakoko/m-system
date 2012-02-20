#include "ruleeditwidget.h"

#include <QToolButton>
#include <QComboBox>
#include <QDebug>
#include "symptompickerdialog.h"



enum Columns {
	symptom,
	// itemOperator,
	value,
	probabilityWithDisease,
	probabilityWithoutDisease,
	count
};


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
