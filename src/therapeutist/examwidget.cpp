#include "examwidget.h"


ExamWidgetFactory ExamWidget::m_factory;


ExamWidget::ExamWidget(const int examId, const QString &textid, const QString& label)
	: m_examId(examId)
	, m_examDataId(InvalidId)
	, m_textid(textid)
	, m_label(label)
{
}


ExamWidget::~ExamWidget()
{

}


QString ExamWidget::label() const
{
	return m_label;
}
