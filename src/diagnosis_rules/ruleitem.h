#pragma once


#include <QObject>
#include <QSqlRecord>
#include <QVariant>


class RuleItem
{
public:
	RuleItem();
	RuleItem(const QSqlRecord& rec);


	// Getters.
	int uiElementId() const
	{	return m_uiElementId;	}

	QVariant textValue() const
	{	return m_textValue;	}
	QVariant realValue() const
	{	return m_realValue;	}
	QVariant enumValue() const
	{	return m_enumValue;	}

	double probabilityWithDisease() const
	{	return m_probabilityWithDisease;	}
	double probabilityWithoutDisease() const
	{	return m_probabilityWithoutDisease;	}


	// Setters.
	void setUiElementId(const int id);

	void setTextValue(const QString& value);
	void setRealValue(const double& value);
	void setEnumValue(const int value);

	void setProbabilityWithDisease(const double& probability);
	void setProbabilityWithoutDisease(const double& probability);


	void save(const int ruleId);
	void deleteRuleItem();

private:
	int m_itemId;
	int m_uiElementId;

	QVariant m_textValue;
	QVariant m_realValue;
	QVariant m_enumValue;

	double m_probabilityWithDisease;
	double m_probabilityWithoutDisease;

};
