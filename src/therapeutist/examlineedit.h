#pragma once

#include "examwidget.h"

class QLabel;
class QLineEdit;

class ExamLineEdit : public ExamWidget
{
	Q_OBJECT
public:
	ExamLineEdit(const int examId, const QString& textId, const QString& label);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

private:
	QLabel* m_label;
	QLineEdit* m_lineEdit;

	bool m_textWasChanged;

private slots:
	void textChanged();

};
