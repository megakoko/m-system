#pragma once

class ExamWidget;
class QString;

class ExamWidgetFactory
{
public:
	ExamWidgetFactory();
	ExamWidget* createWidget(const int examId, const QString& textid) const;
};
