#pragma once

#include "saveablepluginwidget.h"
#include "ui_examinationeditwidget.h"

class ExamContainer;

class ExaminationEditWidget : public SaveablePluginWidget, private Ui::ExaminationEditWidget
{
	Q_OBJECT

public:
	explicit ExaminationEditWidget(const int examinationId, QWidget *parent = 0);

	static QString tabName(const int patientId);

	bool canSave(QString &errorDescription) const;
	void save();

private:
	void init();
	void initConnections();

	static QStringList patientName(const int patientId);

	int m_examinationId;
	int m_patientId;

	ExamContainer* m_mainContainer;

private slots:
	void choosePatient();

};
