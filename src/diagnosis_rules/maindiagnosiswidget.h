#pragma once

#include "ui_maindiagnosiswidget.h"
#include "pluginwidget.h"


class QSqlQueryModel;


class MainDiagnosisWidget : public PluginWidget, private Ui::MainDiagnosisWidget
{
	Q_OBJECT
public:
	explicit MainDiagnosisWidget(QWidget *parent = 0);

private:
	void init();
	void initConnections();

	QSqlQueryModel* m_model;

private slots:
	void addRule();
	void editRule();
	void deleteRule();

	void selectionChanged();

	void updateRuleList();

};
