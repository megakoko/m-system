#pragma once

#include "ui_diagnosishelpdialog.h"

template <class K, class V> class QMap;
class QVariant;
class DsRule;


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

private slots:
	void changeButtonsAccessibility();

};
