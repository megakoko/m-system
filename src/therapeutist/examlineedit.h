#pragma once

#include "examinputwidget.h"

class QLineEdit;

class ExamLineEdit : public ExamInputWidget
{
	Q_OBJECT
public:
	ExamLineEdit(const int examId, const QString& textId);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId);

	QWidget* widget() const;

public slots:
	void resetValue();

protected:
	QLineEdit* m_lineEdit;

private:
	bool m_textIsNull;

private slots:
	void textChanged();

};
