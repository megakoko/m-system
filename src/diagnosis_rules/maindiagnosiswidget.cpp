#include "maindiagnosiswidget.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


#include "macros.h"
#include "ruleeditwidget.h"


MainDiagnosisWidget::MainDiagnosisWidget(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainDiagnosisWidget::init()
{
	m_model = new QSqlQueryModel(this);
	updateRuleList();


	m_view->setModel(m_model);
	m_view->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	m_view->hideColumn(0);

	selectionChanged();
}


void MainDiagnosisWidget::initConnections()
{
	connect(m_addRule, SIGNAL(clicked()), SLOT(addRule()));
	connect(m_editRule, SIGNAL(clicked()), SLOT(editRule()));
	connect(m_deleteRule, SIGNAL(clicked()), SLOT(deleteRule()));

	connect(m_view->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));

	connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(editRule()));
}


void MainDiagnosisWidget::addRule()
{
	// TODO
	RuleEditWidget* w = new RuleEditWidget(RuleEditWidget::InvalidId, this);
	connect(w, SIGNAL(saved()), SLOT(updateRuleList()));
	requestToAddNewWidget(w, "");
}


void MainDiagnosisWidget::editRule()
{
	// TODO
	const int row = m_view->selectionModel()->selectedRows(0).first().row();
	const int id = m_model->record(row).value(0).toInt();

	RuleEditWidget* w = new RuleEditWidget(id, this);
	connect(w, SIGNAL(saved()), SLOT(updateRuleList()));
	requestToAddNewWidget(w, "");
}


void MainDiagnosisWidget::deleteRule()
{
	const int answer = QMessageBox::question(this, "Удаление правила",
											 "Вы действительно хотите удалить правило?",
											 QMessageBox::Yes | QMessageBox::No,
											 QMessageBox::No);
	if(answer == QMessageBox::Yes)
	{
		const int row = m_view->selectionModel()->selectedRows(0).first().row();
		const int id = m_model->record(row).value(0).toInt();

		QSqlQuery q;
		q.prepare("DELETE FROM DsRuleItem WHERE ruleId = ?");
		q.addBindValue(id);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM DsRule WHERE id = ?");
		q.addBindValue(id);
		q.exec();
		checkQuery(q);

		updateRuleList();
	}
}


void MainDiagnosisWidget::selectionChanged()
{
	const bool enableButtons =
			m_view->selectionModel()->hasSelection() ||
			m_view->selectionModel()->selectedRows(0).size() == 1;

	m_editRule->setEnabled(enableButtons);
	m_deleteRule->setEnabled(enableButtons);
}


void MainDiagnosisWidget::updateRuleList()
{
	static const QString query = " SELECT rule.id, COALESCE(diseasetext, mkb.description) "
								 " FROM DsRule rule "
								 " LEFT JOIN Mkb10 mkb ON rule.diseaseMkbId = mkb.id "
								 " ORDER BY rule.id ";

	m_model->setQuery(query);
	checkQuery(m_model->query());
}
