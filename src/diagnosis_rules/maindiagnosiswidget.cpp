#include "maindiagnosiswidget.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
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
}


void MainDiagnosisWidget::addRule()
{
	// TODO
	RuleEditWidget* w = new RuleEditWidget(RuleEditWidget::InvalidId, this);
	requestToAddNewWidget(w, "");
}


void MainDiagnosisWidget::editRule()
{
	// TODO
}


void MainDiagnosisWidget::deleteRule()
{
	// TODO
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
