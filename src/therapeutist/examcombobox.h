#pragma once

#include "examwidget.h"

class QLabel;
class QComboBox;

class ExamComboBox : public ExamWidget
{
public:
	ExamComboBox(const int examId, const QString& textId, const QString& label);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

private:
	QLabel* m_label;
	QComboBox* m_comboBox;

};
