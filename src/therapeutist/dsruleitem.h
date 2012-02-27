#pragma once

#include <QString>

template <class Key, class T> class QMap;
class QVariant;
class QSqlRecord;


class DsRuleItem
{
public:
	DsRuleItem(const QMap<int, QVariant>& data, const QSqlRecord& rec);
	bool hasSymptom() const;

	double probabilityWithDisease() const;
	double probabilityWithoutDisease() const;

	QString fullDescription() const
	{return m_fullDescription;}

private:


	bool m_hasSymptom;

	double m_probabilityWithDisease;
	double m_probabilityWithoutDisease;

	QString m_fullDescription;

};
