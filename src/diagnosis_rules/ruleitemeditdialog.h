#pragma once

#include "ui_ruleitemeditdialog.h"
#include "ruleitem.h"


class RuleItemEditDialog : public QDialog, private Ui::RuleItemEditDialog
{
	Q_OBJECT
public:
	explicit RuleItemEditDialog(const RuleItem& ruleItem, QWidget *parent = 0);
	RuleItem ruleItem();

private:
	void init();
	void initConnections();

	void updateSymptomNameAndValueWidgets();
	void populateOperatorList(const QStringList& operatorTextids);

	bool currentOperatorHasTwoValues() const;

	RuleItem m_ruleItem;

private slots:
	void chooseSymptom();
	void operatorChanged();
	void checkFields();

};
