#include "ruleitem.h"


#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include <QDebug>


RuleItem::RuleItem()
	: m_itemId(0)	// TODO
	, m_symptomLabel(new QLabel)
	, m_symptomButton(new QToolButton)
	, m_itemOperator(new QComboBox)
	, m_value(new QWidget)
	, m_textValue(new QLineEdit)
	, m_realValue(new QDoubleSpinBox)
	, m_enumValue(new QComboBox)
	, m_removeButton(new QToolButton)
{
	init();
	initConnections();
}


void RuleItem::init()
{
	QHBoxLayout* layout = new QHBoxLayout(m_value);
	layout->setContentsMargins(QMargins());
	layout->addWidget(m_textValue);
	layout->addWidget(m_realValue);
	layout->addWidget(m_enumValue);
	m_value->setLayout(layout);


	m_realValue->hide();
	m_enumValue->hide();


	m_symptomButton->setProperty("type", "edit");
	m_removeButton->setProperty("type", "remove");
}


void RuleItem::initConnections()
{
	connect(m_removeButton, SIGNAL(clicked()), SIGNAL(removeMe()));
}


bool RuleItem::canSave() const
{
	return false;
}


void RuleItem::save()
{

}


QLabel* RuleItem::symptomLabel() const
{
	return m_symptomLabel;
}

QToolButton* RuleItem::symptomButton() const
{
	return m_symptomButton;
}

QComboBox* RuleItem::itemOperator() const
{
	return m_itemOperator;
}

QWidget* RuleItem::value() const
{
	return m_value;
}

QToolButton* RuleItem::removeButton() const
{
	return m_removeButton;
}
