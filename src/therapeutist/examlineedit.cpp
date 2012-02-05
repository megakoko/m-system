#include "examlineedit.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"



ExamLineEdit::ExamLineEdit(const int examId, const QString &textId, const QString &labelText)
	: ExamWidget(examId, textId, labelText)
	, m_label(new QLabel(labelText))
	, m_lineEdit(new QLineEdit())
	, m_textIsNull(true)
{
	connect(m_lineEdit, SIGNAL(textChanged(QString)), SLOT(textChanged()));
	connect(m_lineEdit, SIGNAL(textChanged(QString)), SIGNAL(valueChanged()));
	connect(this, SIGNAL(valueChanged()), SLOT(updateLabel()));

	m_lineEdit->setMaxLength(Therapeutist::interfaces->db->
							 fieldMaximumLength("examinationdata", "textvalue"));

	init();
}


QLabel* ExamLineEdit::label() const
{
	return m_label;
}


QWidget* ExamLineEdit::widget() const
{
	return m_lineEdit;
}


bool ExamLineEdit::valueIsNull() const
{
	return m_textIsNull;
}


bool ExamLineEdit::valueCanBeReseted() const
{
	return true;
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

		emit valueChanged();
	}
}


void ExamLineEdit::updateLabel()
{
	if(valueIsNull())
		m_label->setText(m_labelText);
	else
		m_label->setText(QString("<b>%1</b>").arg(m_labelText));
}


void ExamLineEdit::init()
{
	if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT id, textValue FROM ExaminationData "
				  " WHERE examinationId = ? AND "
				  " uiElementId = (SELECT id FROM UiElement WHERE textid = ?) ");
		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			m_examDataId = q.value(0).toInt();
			m_lineEdit->setText(q.value(1).toString());
		}
	}
}


bool ExamLineEdit::save(const int examId) const
{
	bool savedSuccessfully = false;

	// В этом случае сохранять не надо.
	if(m_examDataId == InvalidId && valueIsNull())
		savedSuccessfully = true;
	else
	{
		QSqlQuery q;

		if(m_examDataId == InvalidId && !valueIsNull())
		{
			q.prepare(" INSERT INTO ExaminationData "
					  " (examinationId, uiElementId, textValue) "
					  " VALUES(:examId, (SELECT id FROM UiElement WHERE textid = :textid),"
					  " :textValue) ");
			q.bindValue(":examId", examId);
			q.bindValue(":textid", m_textid);
			q.bindValue(":textValue", m_lineEdit->text());
		}
		else if(m_examDataId != InvalidId && valueIsNull())
		{
			q.prepare(" DELETE FROM ExaminationData "
					  " WHERE id = :id");
			q.bindValue(":id", m_examDataId);
		}
		else if(m_examDataId != InvalidId && !valueIsNull())
		{
			q.prepare(" UPDATE ExaminationData SET textValue = :textValue "
					  " WHERE id = :id");
			q.bindValue(":textValue", m_lineEdit->text());
			q.bindValue(":id", m_examDataId);
		}

		q.exec();
		checkQuery(q);

		savedSuccessfully = q.isActive();
	}

	return savedSuccessfully;
}


void ExamLineEdit::textChanged()
{
	m_textIsNull = false;
}
