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

	int patientAge() const;

private:
	void init();
	void initConnections();

	void savePatientAge();

	static QStringList patientName(const int patientId);

	int m_examinationId;
	int m_patientId;
	int m_therapeutistId;

	ExamContainer* m_mainContainer;

private slots:
	void choosePatient();
	void chooseTherapeutist();

};
