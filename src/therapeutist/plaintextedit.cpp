#include "plaintextedit.h"
#include <QDebug>


PlainTextEdit::PlainTextEdit(QWidget* parent)
	: QPlainTextEdit(parent)
	, m_lastHeight(-100500)
	, m_maxLength(-1)
{
	setTabChangesFocus(true);
	connect(this, SIGNAL(textChanged()), SLOT(resizeWidget()));
	connect(this, SIGNAL(textChanged()), SLOT(chopText()));

	resizeWidget();
}


int PlainTextEdit::maxLength() const
{
	return m_maxLength;
}


void PlainTextEdit::setMaxLength(const int maxLength)
{
	m_maxLength = maxLength;
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


void PlainTextEdit::chopText()
{
	const int length = toPlainText().length();

	if(length > maxLength() && maxLength() != -1)
	{
		QTextCursor cursor(textCursor());

		// Позиция курсора перед удалением.
		const int positionBefore = cursor.position();
		// Сколько символов удаляем.
		const int charsToDelete = length - maxLength();
		// С какого символа удаляем.
		const int removeStart = positionBefore - charsToDelete;


		setPlainText(toPlainText().remove(removeStart, charsToDelete));
		document()->setModified(true);


		// Восстанавливаем позицию курсора.
		cursor.setPosition(positionBefore - charsToDelete);
		setTextCursor(cursor);
	}
}
