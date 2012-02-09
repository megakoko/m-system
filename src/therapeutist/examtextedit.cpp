#include "examtextedit.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"
#include "plaintextedit.h"


ExamTextEdit::ExamTextEdit(const int examId, const QString &textId)
	: ExamInputWidget(examId, textId)
	, m_textEdit(new PlainTextEdit())
{
	connect(m_textEdit, SIGNAL(textChanged()), SLOT(textChanged()));

	m_textEdit->setMaxLength(Therapeutist::interfaces->db->
							 fieldMaximumLength("examinationdata", "textvalue"));

	init();
}


QWidget* ExamTextEdit::widget() const
{
	return m_textEdit;
}


bool ExamTextEdit::valueIsNull() const
{
	return !m_textEdit->document()->isModified();
}


QString ExamTextEdit::value() const
{
	return m_textEdit->toPlainText();
}


void ExamTextEdit::resetValue()
{
	if(!valueIsNull())
	{
		m_textEdit->clear();

		emit valueChanged(valueIsNull());
	}
}


void ExamTextEdit::textChanged()
{
	emit valueChanged(valueIsNull());
}


void ExamTextEdit::init()
{
	const QString& value = initHelpher("textValue").toString();
	if(!value.isNull())
	{
		m_textEdit->setPlainText(value);
		m_textEdit->document()->setModified(true);
	}
}


bool ExamTextEdit::save(const int examId)
{
	return saveHelper("textValue", examId, m_textEdit->toPlainText());
}
