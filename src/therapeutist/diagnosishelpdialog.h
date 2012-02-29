#pragma once

#include "ui_diagnosishelpdialog.h"
#include "dsrule.h"

template <class K, class V> class QMap;
class QVariant;


class DiagnosisHelpDialog : public QDialog, private Ui::DiagnosisHelpDialog
{
	Q_OBJECT	
public:
	explicit DiagnosisHelpDialog(const QMap<int, QVariant>& data);

	QString selectedDiagnosis() const;

private:
	void init(const QMap<int, QVariant>& data);
	void initConnections();

	void addDiagnosisToTable(const DsRule& rule);

	QList<DsRule> m_rules;

private slots:
	void changeButtonsAccessibility();
	void toggleHiddenDiagnoses(const bool showDiagnoses);

};
