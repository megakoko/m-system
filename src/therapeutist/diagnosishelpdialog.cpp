#include "diagnosishelpdialog.h"

#include <QPushButton>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "macros.h"


namespace TableColumns
{
	enum {
		Diagnosis,
		Probability,
		Count
	};
}


QString formatProbability(const double &probability)
{
	// Вероятность [0;1] переводим в проценты.
	return QLocale().toString(probability * 100.0, 'f', 2);
}


DiagnosisHelpDialog::DiagnosisHelpDialog(const QMap<int, QVariant> &data)
{
	setupUi(this);
	init(data);
	initConnections();
}


void DiagnosisHelpDialog::init(const QMap<int, QVariant> &data)
{
	m_diagnosisTable->setColumnCount(TableColumns::Count);
	m_diagnosisTable->horizontalHeader()->
			setResizeMode(TableColumns::Diagnosis, QHeaderView::Stretch);
	m_diagnosisTable->horizontalHeader()->
			setResizeMode(TableColumns::Probability, QHeaderView::ResizeToContents);
	m_diagnosisTable->setHorizontalHeaderLabels(QStringList() << "Диагноз" << "Вероятность, %");

	changeButtonsAccessibility();


	QSqlQuery q("SELECT id, diseaseText, diseaseProbability FROM DsRule");
	while(q.next())
		m_rules << DsRule(data, q.record());

	qSort(m_rules.begin(), m_rules.end(), qGreater<DsRule>());

	foreach(const DsRule& rule, m_rules)
		addDiagnosisToTable(rule);
}


void DiagnosisHelpDialog::initConnections()
{
	connect(m_showAllDiagnosis, SIGNAL(toggled(bool)), SLOT(toggleHiddenDiagnoses(bool)));

	connect(m_diagnosisTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()));
	connect(m_diagnosisTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(changeButtonsAccessibility()));
}


void DiagnosisHelpDialog::addDiagnosisToTable(const DsRule &rule)
{
	const int row = m_diagnosisTable->rowCount();
	m_diagnosisTable->insertRow(row);
	if(!rule.hasAllSymptoms())
		m_diagnosisTable->hideRow(row);


	static const QChar V(0x2713);
	static const QChar X(0x2717);

	QStringList toolTipParts;
	foreach(const DsRuleItem& ruleItem, rule.ruleItems())
		toolTipParts << QString("<span style='color: %1'>%2</span> %3").
						arg(ruleItem.hasSymptom() ? "green" : "red").
						arg(ruleItem.hasSymptom() ? V : X).
						arg(ruleItem.fullDescription());

	const QString& toolTip = toolTipParts.join("<br/>");


	QTableWidgetItem* item;

	item = new QTableWidgetItem(rule.diseaseText());
	item->setToolTip(toolTip);
	m_diagnosisTable->setItem(row, TableColumns::Diagnosis, item);

	item = new QTableWidgetItem;
	if(rule.hasAllSymptoms())
		item->setText(formatProbability(rule.probabilityOfDiseaseGivenSymptoms()));
	item->setToolTip(toolTip);
	m_diagnosisTable->setItem(row, TableColumns::Probability, item);
}


void DiagnosisHelpDialog::changeButtonsAccessibility()
{
	const bool singleDiagnosisSelected =
			m_diagnosisTable->selectionModel() != NULL &&
			m_diagnosisTable->selectionModel()->selectedRows(0).size() == 1;

	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(singleDiagnosisSelected);
}


QString DiagnosisHelpDialog::selectedDiagnosis() const
{
	QString diagnosis;

	if(m_diagnosisTable->selectionModel() != NULL)
	{
		const QModelIndexList& indexes = m_diagnosisTable->selectionModel()->selectedRows(0);
		if(indexes.size() == 1)
		{
			const int row = indexes.first().row();
			const int col = indexes.first().column();

			diagnosis = m_diagnosisTable->item(row, col)->text();
		}
	}

	return diagnosis;
}


void DiagnosisHelpDialog::toggleHiddenDiagnoses(const bool showDiagnoses)
{
	for(int row = 0; row < m_diagnosisTable->rowCount(); ++row)
		if(!m_rules.at(row).hasAllSymptoms())
			m_diagnosisTable->setRowHidden(row, !showDiagnoses);
}
