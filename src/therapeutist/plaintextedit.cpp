#include "plaintextedit.h"
#include <QDebug>


PlainTextEdit::PlainTextEdit(QWidget* parent)
	: QPlainTextEdit(parent)
	, m_lastHeight(-100500)
{
	setTabChangesFocus(true);
	connect(this, SIGNAL(textChanged()), SLOT(resizeWidget()));

	resizeWidget();
}


int PlainTextEdit::requiredHeight() const
{
	const int lines = document()->size().height();
	static const int lineHeight = QFontMetrics(font()).lineSpacing();

	return (lines+1) * lineHeight;
}


void PlainTextEdit::resizeWidget()
{
	const int widgetRequiredHeigth = requiredHeight();

	if(widgetRequiredHeigth != m_lastHeight)
	{
		// TODO: некрасиво.
		setMaximumHeight(widgetRequiredHeigth);
		setMinimumHeight(widgetRequiredHeigth);

		m_lastHeight = widgetRequiredHeigth;
	}
}
