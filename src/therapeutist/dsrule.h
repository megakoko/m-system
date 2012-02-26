#pragma once

#include <QString>

template <class K, class V> class QMap;
class QVariant;
class QSqlRecord;


class DsRule
{
public:
	DsRule(const QMap<int, QVariant> data, const QSqlRecord& record);

	double probabilityOfDiseaseGivenSymptoms() const;
	QString diseaseText() const;

private:
	double m_probability;
	QString m_diseaseText;

};

bool operator<(const DsRule& left, const DsRule& right);

