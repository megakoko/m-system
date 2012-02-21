#pragma once

#include "saveablepluginwidget.h"
#include "ui_ruleeditwidget.h"
#include "ruleitem.h"



class RuleEditWidget : public SaveablePluginWidget, private Ui::RuleEditWidget
{
	Q_OBJECT
public:
	RuleEditWidget(QWidget *parent = 0);

	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();
	void initConnections();

	static QString formatProbability(const double& probability);

	QList<RuleItem> m_ruleItems;
	QList<RuleItem> m_removedRuleItems;

private slots:
	void addRuleItem();
	void removeRuleItem();

};
