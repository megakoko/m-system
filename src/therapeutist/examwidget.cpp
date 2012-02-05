#include "examwidget.h"


ExamWidgetFactory ExamWidget::m_factory;


ExamWidget::ExamWidget(const int examId, const QString &textid, const QString& labelText)
	: m_examId(examId)
	, m_examDataId(InvalidId)
	, m_textid(textid)
	, m_labelText(labelText)
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


void ExamWidget::resetValue()
{
	// Ничего не делаем.
}
