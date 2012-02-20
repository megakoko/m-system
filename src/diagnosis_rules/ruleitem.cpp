#include "ruleitem.h"


#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>


#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"


RuleItem::RuleItem()
	: m_itemId(0) // TODO
{

}


RuleItem::RuleItem(const QSqlRecord &rec)
	: m_itemId(rec.value("id").toInt())
	, m_uiElementId(rec.value("uiElementId").toInt())
	, m_textValue(rec.value("textValue"))
	, m_realValue(rec.value("realValue"))
	, m_enumValue(rec.value("enumValue"))
	, m_probabilityWithDisease(rec.value("probabilityWithDisease").toDouble())
	, m_probabilityWithoutDisease(rec.value("probabilityWithoutDisease").toDouble())
{
}


void RuleItem::save(const int ruleId)
{
	QSqlQuery q;
	if(m_itemId == 0)	// TODO
	{
		q.prepare(" INSERT INTO DsRuleItem "
				  " ( ruleId,  uiElementId,  textValue,  realValue,  enumValue, "
				  "  probabilityWithDisease,  probabilityWithoutDisease) VALUES "
				  " (:ruleId, :uiElementId, :textValue, :realValue, :enumValue, "
				  " :probabilityWithDisease, :probabilityWithoutDisease) ");
	}
	else
	{
		q.prepare(" UPDATE DsRuleItem SET "
				  " ruleId = :ruleId, "
				  " uiElementId = :uiElementId, "
				  " textValue = :textValue, "
				  " realValue = :realValue, "
				  " enumValue = :enumValue, "
				  " probabilityWithDisease = :probabilityWithDisease, "
				  " probabilityWithoutDisease = :probabilityWithoutDisease "
				  " WHERE id = :id ");
		q.bindValue(":id", m_itemId);
	}

	q.bindValue(":ruleId", ruleId);
	q.bindValue(":uiElementId", m_uiElementId);
	q.bindValue(":textValue", m_textValue);
	q.bindValue(":realValue", m_realValue);
	q.bindValue(":enumValue", m_enumValue);
	q.bindValue(":probabilityWithDisease", m_probabilityWithDisease);
	q.bindValue(":probabilityWithoutDisease", m_probabilityWithoutDisease);
	q.exec();
	checkQuery(q);
}


void RuleItem::deleteRuleItem()
{
	if(m_itemId != 0)	// TODO
	{
		QSqlQuery q;
		q.prepare("DELETE FROM DsRuleItem WHERE id = ?");
		q.addBindValue(m_itemId);
		q.exec();
		checkQuery(q);
	}
}


void RuleItem::setUiElementId(const int id)
{
	m_uiElementId = id;
}


void RuleItem::setTextValue(const QString &value)
{
	m_textValue = value;
	m_realValue.clear();
	m_enumValue.clear();
}


void RuleItem::setRealValue(const double &value)
{
	m_textValue.clear();
	m_realValue = value;
	m_enumValue.clear();
}


void RuleItem::setEnumValue(const int value)
{
	m_textValue.clear();
	m_realValue.clear();
	m_enumValue = value;
}


void RuleItem::setProbabilityWithDisease(const double& probability)
{
	if(0.0 <= probability && probability <= 1.0)
		m_probabilityWithDisease = probability;
	else
		qWarning() << "Invalid probability in" << __FUNCTION__;
}


void RuleItem::setProbabilityWithoutDisease(const double& probability)
{
	if(0.0 <= probability && probability <= 1.0)
		m_probabilityWithoutDisease = probability;
	else
		qWarning() << "Invalid probability in" << __FUNCTION__;
}
