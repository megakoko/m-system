#pragma once

#include "examinputwidget.h"

class PlainTextEdit;

class ExamTextEdit : public ExamInputWidget
{
	Q_OBJECT
public:
	ExamTextEdit(const int examId, const QString& textId);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

	QWidget* widget() const;

public slots:
	void resetValue();

private:
	PlainTextEdit* m_textEdit;

private slots:
	void textChanged();

};
