#pragma once

#include "examinputwidget.h"

class QLineEdit;

class ExamLineEdit : public ExamInputWidget
{
	Q_OBJECT
public:
	ExamLineEdit(const int examId, const QString& textId, const QString& labelText);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	bool valueCanBeReseted() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

	QWidget* widget() const;

public slots:
	void resetValue();

private:
	QLineEdit* m_lineEdit;

	bool m_textIsNull;

private slots:
	void textChanged();

};
