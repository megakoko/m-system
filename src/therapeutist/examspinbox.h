#pragma once

#include "examinputwidget.h"

class QDoubleSpinBox;

class ExamSpinBox : public ExamInputWidget
{
	Q_OBJECT
public:
	ExamSpinBox(const int examId, const QString& textId);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

	QWidget* widget() const;

public slots:
	void resetValue();

private:
	QDoubleSpinBox* m_spinBox;

private slots:
	void textChanged();

};
