#include "examspinbox.h"

#include <QDoubleSpinBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"


ExamSpinBox::ExamSpinBox(const int examId, const QString &textId)
	: ExamInputWidget(examId, textId)
	, m_spinBox(new QDoubleSpinBox())
{
	connect(m_spinBox, SIGNAL(valueChanged(double)), SLOT(textChanged()));

	m_spinBox->setSingleStep(0.1);
	m_spinBox->setMinimum(0.0);
	m_spinBox->setMaximum(10000000);
	m_spinBox->setSpecialValueText("[Значение отсутствует]");

	init();
}


QWidget* ExamSpinBox::widget() const
{
	return m_spinBox;
}


bool ExamSpinBox::valueIsNull() const
{
	return m_spinBox->value() == m_spinBox->minimum();
}


QString ExamSpinBox::value() const
{
	return m_spinBox->text();
}


void ExamSpinBox::resetValue()
{
	if(!valueIsNull())
	{
		m_spinBox->setValue(m_spinBox->minimum());

		emit valueChanged(valueIsNull());
	}
}


void ExamSpinBox::textChanged()
{
	emit valueChanged(valueIsNull());
}


void ExamSpinBox::init()
{
	const double value = initHelpher("realValue").toDouble();
	m_spinBox->setValue(value);
}


bool ExamSpinBox::save(const int examId)
{
	return saveHelper("realValue", examId, m_spinBox->value());
}
