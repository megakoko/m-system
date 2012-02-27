#include "dsruleitem.h"

#include <QMap>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>


DsRuleItem::DsRuleItem(const QMap<int, QVariant> &data, const QSqlRecord &rec)
	: m_hasSymptom(false)
	, m_probabilityWithDisease(rec.value("probabilityWithDisease").toDouble())
	, m_probabilityWithoutDisease(rec.value("probabilityWithoutDisease").toDouble())
{
	const int uiElementId = rec.value("uiElementId").toInt();

	// values.
	const QVariant& value = data.value(uiElementId);

	const QVariant& ruleTextValue = rec.value("textValue");
	const QVariant& ruleRealValue = rec.value("realValue");
	const QVariant& ruleRealValue2= rec.value("realValue2");
	const QVariant& ruleEnumValue = rec.value("enumValue");


	if(data.contains(uiElementId))
	{
		bool operatorWasAppliedSuccessfully = true;

		const QString& operatorTextid = rec.value("textid").toString();
		if(operatorTextid == "equal")
		{
			// text
			if(value.type() == QVariant::String && !ruleTextValue.isNull())
				m_hasSymptom = (value.toString() == ruleTextValue.toString());
			// enum
			else if(value.type() == QVariant::Int && !ruleEnumValue.isNull())
				m_hasSymptom = (value.toInt() == ruleEnumValue.toInt());
			// real
			else if(value.type() == QVariant::Double && !ruleRealValue.isNull())
				m_hasSymptom = qFuzzyCompare(value.toDouble(), ruleRealValue.toDouble());
			else
				operatorWasAppliedSuccessfully = false;
		}
		else if(operatorTextid == "less")
		{
			// real
			if(value.type() == QVariant::Double && !ruleRealValue.isNull())
				m_hasSymptom = (value.toDouble() < ruleRealValue.toDouble());
			else
				operatorWasAppliedSuccessfully = false;
		}
		else if(operatorTextid == "more")
		{
			// real
			if(value.type() == QVariant::Double && !ruleRealValue.isNull())
				m_hasSymptom = (value.toDouble() > ruleRealValue.toDouble());
			else
				operatorWasAppliedSuccessfully = false;
		}
		else if(operatorTextid == "between")
		{
			// real
			if(value.type() == QVariant::Double &&
			   !ruleRealValue.isNull() && !ruleRealValue2.isNull())
			{
				m_hasSymptom = (value.toDouble() > ruleRealValue.toDouble() &&
								value.toDouble() < ruleRealValue2.toDouble());
			}
			else
				operatorWasAppliedSuccessfully = false;
		}


		if(!operatorWasAppliedSuccessfully)
		{
			qWarning() << "Failed to apply operator" << operatorTextid <<
						  "on following value:" << value;
			qWarning() << "Rule values: text -" << ruleTextValue <<
						  ", real -" << ruleRealValue << ruleRealValue2 <<
						  ", enum -" << ruleEnumValue;
		}
	}


	const QString& label = rec.value("label").toString();
	const QVariant& enumText = rec.value("value");
	const QString& sign = rec.value("sign").toString().simplified();


	QString val;
	if(!ruleTextValue.isNull())
		val = ruleTextValue.toString();
	else if(!ruleRealValue.isNull())
	{
		if(ruleRealValue2.isNull())
			val = QString::number(ruleRealValue.toDouble());
		else
			val = QString("[%1;%2]").arg(ruleRealValue.toDouble()).arg(ruleRealValue2.toDouble());
	}
	else if(!ruleEnumValue.isNull())
		val = enumText.toString();


	m_fullDescription = label + " " + sign + " " + val;
	m_fullDescription.replace("<", "&lt;");
	m_fullDescription.replace(">", "&gt;");
}


bool DsRuleItem::hasSymptom() const
{
	return m_hasSymptom;
}


double DsRuleItem::probabilityWithDisease() const
{
	return m_probabilityWithDisease;
}


double DsRuleItem::probabilityWithoutDisease() const
{
	return m_probabilityWithoutDisease;
}
