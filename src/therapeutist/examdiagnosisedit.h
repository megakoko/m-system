#pragma once

#include "examlineedit.h"
class ExamContainer;

class ExamDiagnosisEdit : public ExamLineEdit
{
	Q_OBJECT
public:
	ExamDiagnosisEdit(const int examId, const QString& textid);

	QWidget* widget() const;

	void setMainContainer(ExamContainer *container);

private:
	QWidget* m_widget;

	ExamContainer* m_mainContainer;

private slots:
	void openDiagnosisHelp();

};
