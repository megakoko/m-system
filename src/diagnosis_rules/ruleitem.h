#pragma once

#include "saveableobject.h"
#include <QVariant>

class QSqlRecord;


class RuleItem : public SaveableObject
{
public:
	RuleItem();
	RuleItem(const QSqlRecord& rec);


	QString symptomName() const;


	// Getters.
	int uiElementId() const
	{	return m_uiElementId;	}

	QVariant textValue() const
	{	return m_textValue;	}
	QVariant realValue() const
	{	return m_realValue;	}
	QVariant enumValue() const
	{	return m_enumValue;	}

	QString value() const;

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

	void setRuleid(const int ruleId);
	void save();
	void deleteRuleItem();

private:
	int m_itemId;
	int m_uiElementId;
	int m_ruleId;

	QVariant m_textValue;
	QVariant m_realValue;
	QVariant m_enumValue;

	double m_probabilityWithDisease;
	double m_probabilityWithoutDisease;

};
