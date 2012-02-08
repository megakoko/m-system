#include "examlineedit.h"

#include <QLineEdit>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"


ExamLineEdit::ExamLineEdit(const int examId, const QString &textId)
	: ExamInputWidget(examId, textId)
	, m_lineEdit(new QLineEdit())
	, m_textIsNull(true)
{
	connect(m_lineEdit, SIGNAL(textChanged(QString)), SLOT(textChanged()));

	m_lineEdit->setMaxLength(Therapeutist::interfaces->db->
							 fieldMaximumLength("examinationdata", "textvalue"));

	init();
}


QWidget* ExamLineEdit::widget() const
{
	return m_lineEdit;
}


bool ExamLineEdit::valueIsNull() const
{
	return m_textIsNull;
}


QString ExamLineEdit::value() const
{
	return m_lineEdit->text();
}


void ExamLineEdit::resetValue()
{
	if(!valueIsNull())
	{
		m_lineEdit->clear();
		m_textIsNull = true;

		emit valueChanged(valueIsNull());
	}
}


void ExamLineEdit::textChanged()
{
	m_textIsNull = false;
	emit valueChanged(valueIsNull());
}


void ExamLineEdit::init()
{
	const QString& value = initHelpher("textValue").toString();
	m_lineEdit->setText(value);
}


bool ExamLineEdit::save(const int examId) const
{
	return saveHelper("textValue", examId, m_lineEdit->text());
}
