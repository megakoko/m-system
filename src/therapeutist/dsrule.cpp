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
		Последовательно вычисляем вероятность по следующей формуле для i-го симптома,
		затем присваем полученное значение вероятности P(D) и вычисляем для следующего симптома.


							P(D) * P(Si|D)
		P(D|Si) = --------------------------------- * Ky
				  P(D) * P(Si|D) + P(^D) * P(Si|^D)

		где:
		D 			- событие, выражающееся в наличии болезни.
		^D			- событие, выражающееся в отсутствии болезни.
		Si			- событие, выражающееся в проявлении i-го симптома.

		P(D)		- вероятность болезни.
		P(Si|D)		- вероятность проявления i-го симптома при наличии болезни.
		P(Si|^D)	- вероятность проявления i-го симптома при отсутствии болезни.

		Ky			- коэффициент уверенности. Если симптом есть у пациента, равен 1.0,
					  иначе какому-то малому значению (например, 0.01).
	*/


	m_probability = record.value("diseaseProbability").toDouble();


	qDebug() << m_diseaseText;
	qDebug() << m_probability;
	foreach(const DsRuleItem& item, m_ruleItems)
	{
		if(!item.hasSymptom())
			continue;

//		const double ky = item.hasSymptom() ? 1.0 : 0.1;

		const double product1 = m_probability			* item.probabilityWithDisease();
		const double product2 = (1.0 - m_probability)	* item.probabilityWithoutDisease();

		m_probability = /*ky * */ product1 / (product1 + product2);
		qDebug() << m_probability;
	}
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
