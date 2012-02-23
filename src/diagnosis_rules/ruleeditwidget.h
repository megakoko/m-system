#pragma once

#include "saveablepluginwidget.h"
#include "ui_ruleeditwidget.h"
#include "ruleitem.h"



class RuleEditWidget : public SaveablePluginWidget, private Ui::RuleEditWidget
{
	Q_OBJECT
public:
	RuleEditWidget(const int ruleId, QWidget *parent = 0);

	QString tabName() const;

	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();
	void initConnections();

	void addRuleToTable(const RuleItem& ruleItem);

	static QString formatProbability(const double& probability);

	int m_ruleId;

	QList<RuleItem> m_ruleItems;
	QList<RuleItem> m_removedRuleItems;

private slots:
	void updateTabName();

	void addRuleItem();
	void editRuleItem();
	void removeRuleItem();

	void ruleItemSelectionChanged();

};
