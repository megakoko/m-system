#pragma once

#include "examwidget.h"

class QLabel;
class QLineEdit;

class ExamLineEdit : public ExamWidget
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

	QLabel* label() const;
	QWidget* widget() const;

public slots:
	void resetValue();

private:
	QLabel* m_label;
	QLineEdit* m_lineEdit;

	bool m_textIsNull;

private slots:
	void textChanged();

};
