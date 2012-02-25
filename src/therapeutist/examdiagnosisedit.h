#pragma once

#include "examlineedit.h"


class ExamDiagnosisEdit : public ExamLineEdit
{
	Q_OBJECT
public:
	ExamDiagnosisEdit(const int examId, const QString& textid);

	QWidget* widget() const;

private:
	QWidget* m_widget;

private slots:
	void openDiagnosisHelp();

};
