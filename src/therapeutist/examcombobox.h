#pragma once

#include "examinputwidget.h"

class QComboBox;

class ExamComboBox : public ExamInputWidget
{
	Q_OBJECT
public:
	ExamComboBox(const int examId, const QString& textId);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId);

	QWidget* widget() const;

public slots:
	void resetValue();

	void comboBoxIndexChanged();

private:
	QComboBox* m_comboBox;

};
