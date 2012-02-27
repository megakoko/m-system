#include "dsrule.h"

#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"
#include "dsruleitem.h"


DsRule::DsRule(const QMap<int, QVariant> data, const QSqlRecord& record)
{
	m_diseaseText = record.value("diseaseText").toString();


	QSqlQuery q;
	q.prepare(" SELECT uiElementId, label, op.textid, op.sign, textValue, realValue, realValue2, "
			  " enumValue, enums.value, probabilityWithDisease, probabilityWithoutDisease "
			  " FROM DsRuleItem item LEFT JOIN Operator op ON item.operatorId = op.id "
			  " LEFT JOIN UiElement ui ON item.uiElementId = ui.id "
			  " LEFT JOIN UiElementEnums enums ON item.enumValue = enums.id "
			  " WHERE ruleId = ? ");
	q.addBindValue(record.value("id"));
	q.exec();
	checkQuery(q);

	while(q.next())
		m_ruleItems << DsRuleItem(data, q.record());


	/*
		Вычисляем вероятность по следующей формуле:

											P(D) * П[P(Si|D)]
		P(D|S1 & S2 & ... & S3) = --------------------------------------
								  P(D) * П[P(Si|D)] + P(^D) * П[P(Si|^D)]

		где:
		D - событие, выражающееся в наличии болезни.
		^D - событие, выражающееся в отсутствии болезни.
		Si - событие, выражающееся в проявлении i-го симптома.
		P(D) - вероятность болезни.
		P(Si|D) - вероятность проявления i-го симптома при наличии болезни.
		P(Si|^D) - вероятность проявления i-го симптома при отсутствии болезни.

		Далее будут вычислены произведения П[P(Si|D)] и П[P(Si|^D)].
	*/

	const double diseaseProbability = record.value("diseaseProbability").toDouble();

	double product1 = diseaseProbability;
	double product2 = 1.0 - diseaseProbability;

	foreach(const DsRuleItem& item, m_ruleItems)
	{
		if(item.hasSymptom())
		{
			product1 *= item.probabilityWithDisease();
			product2 *= item.probabilityWithoutDisease();
		}
	}

	m_probability = product1 / (product1 + product2);
}


double DsRule::probabilityOfDiseaseGivenSymptoms() const
{
	return m_probability;
}


QString DsRule::diseaseText() const
{
	return m_diseaseText;
}


bool operator< (const DsRule &left, const DsRule &right)
{
	return	left.probabilityOfDiseaseGivenSymptoms() <
			right.probabilityOfDiseaseGivenSymptoms();
}
