#pragma once

#include "examwidget.h"

class QLabel;
class QComboBox;

class ExamComboBox : public ExamWidget
{
public:
	ExamComboBox(const int examId, const QString& textId, const QString& labelText);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

	QLabel* label() const;
	QWidget* widget() const;

private:
	QLabel* m_label;
	QComboBox* m_comboBox;

};