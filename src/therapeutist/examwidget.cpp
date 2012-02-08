#include "examwidget.h"


ExamWidgetFactory ExamWidget::m_factory;


ExamWidget::ExamWidget(const int examId, const QString &textid)
	: m_examId(examId)
	, m_examDataId(InvalidId)
	, m_textid(textid)
{
}


ExamWidget::~ExamWidget()
{

}


bool ExamWidget::valueCanBeReseted() const
{
	return false;
}


QString ExamWidget::labelText() const
{
	return m_labelText;
}


QString ExamWidget::shortLabelText() const
{
	return m_shortLabelText;
}


void ExamWidget::setLabelText(const QString &labelText, const QString& shortLabelText)
{
	m_labelText = labelText;
	m_shortLabelText = shortLabelText;
}


void ExamWidget::resetValue()
{
	// Ничего не делаем.
}
