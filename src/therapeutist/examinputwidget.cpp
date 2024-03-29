#include "examinputwidget.h"

#include <QLabel>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"


ExamInputWidget::ExamInputWidget(const int examId, const QString &textId)
	: ExamWidget(examId, textId)
	, m_label(new QLabel())
{
	connect(this, SIGNAL(valueChanged(bool)), SLOT(updateLabelStyle()));
}


void ExamInputWidget::setLabelText(const QString &labelText, const QString& shortLabelText)
{
	ExamWidget::setLabelText(labelText, shortLabelText);
	updateLabelStyle();
}


QLabel* ExamInputWidget::label() const
{
	return m_label;
}


bool ExamInputWidget::valueCanBeReseted() const
{
	return true;
}


void ExamInputWidget::updateLabelStyle()
{
	const QString& text = (shortLabelText().isNull() ? labelText() : shortLabelText());

	if(valueIsNull())
		m_label->setText(text);
	else
		m_label->setText(QString("<b>%1</b>").arg(text));
}


QVariant ExamInputWidget::initHelpher(const QString& fieldName)
{
	QVariant value;
	if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT id, " + fieldName + " FROM ExaminationData "
				  " WHERE examinationId = ? AND "
				  " uiElementId = (SELECT id FROM UiElement WHERE textid = ?) ");
		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			m_examDataId = q.value(0).toInt();
			value = q.value(1);
		}
	}

	return value;
}


bool ExamInputWidget::saveHelper(const QString& fieldName, const int examId, const QVariant& value)
{

	bool savedSuccessfully = false;

	// В этом случае сохранять не надо.
	if(m_examDataId == InvalidId && valueIsNull())
		savedSuccessfully = true;
	else
	{
		qDebug() << m_examDataId;
		QSqlQuery q;

		if(m_examDataId == InvalidId && !valueIsNull())
		{
			q.prepare(" INSERT INTO ExaminationData "
					  " (examinationId, uiElementId, " + fieldName + ") "
					  " VALUES(:examId, (SELECT id FROM UiElement WHERE textid = :textid),"
					  " :value) " +
					  Therapeutist::interfaces->db->returningSentence("id"));
			q.bindValue(":examId", examId);
			q.bindValue(":textid", m_textid);
			q.bindValue(":value", value);
		}
		else if(m_examDataId != InvalidId && valueIsNull())
		{
			q.prepare(" DELETE FROM ExaminationData "
					  " WHERE id = :id");
			q.bindValue(":id", m_examDataId);
		}
		else if(m_examDataId != InvalidId && !valueIsNull())
		{
			q.prepare(" UPDATE ExaminationData SET " + fieldName + " = :value "
					  " WHERE id = :id");
			q.bindValue(":value", value);
			q.bindValue(":id", m_examDataId);
		}

		q.exec();
		checkQuery(q);


		if(m_examDataId == InvalidId)
			m_examDataId = Therapeutist::interfaces->db->lastInsertedId(&q).toInt();


		savedSuccessfully = q.isActive();
	}

	return savedSuccessfully;
}
