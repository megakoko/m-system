#pragma once

#include "ui_maintherapeutistwidget.h"
#include "pluginwidget.h"

class QSqlQueryModel;



class MainTherapeutistWidget : public PluginWidget, private Ui::MainTherapeutistWidget
{
	Q_OBJECT

public:
	explicit MainTherapeutistWidget(QWidget* parent = 0);

private:
	void init();
	void initConnections();

	int numberOfSelectedExams() const;

	int selectedExamId() const;

	static int examIdToPatientId(const int examId);
	static QString examinationListQuery();

	/// Возвращает true, если текущий пользователь связан с терапевтом.
	static bool currentUserIsAssociatedWithTherapeutist();

	QSqlQueryModel* m_queryModel;

private slots:
	void updateExaminationList();

	void examSelectionChanged();

	void addExam();
	void editExam();
	void deleteExam();

	void openExamPreview();

};
