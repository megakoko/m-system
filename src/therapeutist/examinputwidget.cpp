#include "examinputwidget.h"

#include <QLabel>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"


ExamInputWidget::ExamInputWidget(const int examId, const QString &textId,
								 const QString &labelText)
	: ExamWidget(examId, textId, labelText)
	, m_label(new QLabel(labelText))
{
	connect(this, SIGNAL(valueChanged(bool)), SLOT(updateLabelStyle()));
}


QLabel* ExamInputWidget::label() const
{
	return m_label;
}


void ExamInputWidget::updateLabelStyle()
{
	if(valueIsNull())
		m_label->setText(m_labelText);
	else
		m_label->setText(QString("<b>%1</b>").arg(m_labelText));
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


bool ExamInputWidget::saveHelper(const QString& fieldName, const int examId, const QVariant& value) const
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
					  " (examinationId, uiElementId, " + fieldName + ") "
					  " VALUES(:examId, (SELECT id FROM UiElement WHERE textid = :textid),"
					  " :value) ");
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

		savedSuccessfully = q.isActive();
	}

	return savedSuccessfully;
}
