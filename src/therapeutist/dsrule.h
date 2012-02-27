#pragma once

#include <QString>
#include <QList>
#include "dsruleitem.h"

template <class K, class V> class QMap;
class QVariant;
class QSqlRecord;


class DsRule
{
public:
	DsRule(const QMap<int, QVariant> data, const QSqlRecord& record);

	double probabilityOfDiseaseGivenSymptoms() const;
	QString diseaseText() const;

	QList<DsRuleItem> ruleItems() const
	{
		return m_ruleItems;
	}

private:
	double m_probability;
	QString m_diseaseText;
	QList<DsRuleItem> m_ruleItems;

};

bool operator<(const DsRule& left, const DsRule& right);

