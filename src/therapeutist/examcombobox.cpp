#include "examcombobox.h"

#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


ExamComboBox::ExamComboBox(const int examId, const QString &textId, const QString &labelText)
	: ExamWidget(examId, textId, labelText)
	, m_label(new QLabel(labelText))
	, m_comboBox(new QComboBox())
{
	connect(m_comboBox, SIGNAL(currentIndexChanged(int)), SIGNAL(valueChanged()));
	connect(this, SIGNAL(valueChanged()), SLOT(updateLabel()));


	QSqlQuery q;
	q.prepare(" SELECT id, value FROM UiElementEnums "
			  " WHERE uiElementTextId = ? ORDER BY id ");
	q.addBindValue(m_textid);
	q.exec();
	checkQuery(q);

	while(q.next())
		m_comboBox->addItem(q.value(1).toString(), q.value(0));
	m_comboBox->setCurrentIndex(-1);

	init();
}


QLabel* ExamComboBox::label() const
{
	return m_label;
}


QWidget* ExamComboBox::widget() const
{
	return m_comboBox;
}


bool ExamComboBox::valueIsNull() const
{
	return (m_comboBox->currentIndex() == -1);
}


bool ExamComboBox::valueCanBeReseted() const
{
	return true;
}


QString ExamComboBox::value() const
{
	return m_comboBox->currentText();
}


void ExamComboBox::resetValue()
{
	if(!valueIsNull())
	{
		m_comboBox->setCurrentIndex(-1);
		emit valueChanged();
	}
}


void ExamComboBox::updateLabel()
{
	if(valueIsNull())
		m_label->setText(m_labelText);
	else
		m_label->setText(QString("<b>%1</b>").arg(m_labelText));
}


void ExamComboBox::init()
{
	if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT id, enumValue FROM ExaminationData "
				  " WHERE examinationId = ? "
				  " AND uiElementId = (SELECT id FROM UiElement WHERE textid = ?) ");
		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			m_examDataId = q.value(0).toInt();
			m_comboBox->setCurrentIndex(m_comboBox->findData(q.value(1)));
		}
	}
}


bool ExamComboBox::save(const int examId) const
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
					  " (examinationId, uiElementId, enumValue) "
					  " VALUES(:examId, (SELECT id FROM UiElement WHERE textid = :textid),"
					  " :enumValue) ");
			q.bindValue(":examId", examId);
			q.bindValue(":textid", m_textid);
			q.bindValue(":enumValue", m_comboBox->itemData(m_comboBox->currentIndex()));
		}
		else if(m_examDataId != InvalidId && valueIsNull())
		{
			q.prepare(" DELETE FROM ExaminationData "
					  " WHERE id = :id");
			q.bindValue(":id", m_examDataId);
		}
		else if(m_examDataId != InvalidId && !valueIsNull())
		{
			q.prepare(" UPDATE ExaminationData SET enumValue = :enumValue "
					  " WHERE id = :id");
			q.bindValue(":enumValue", m_comboBox->itemData(m_comboBox->currentIndex()));
			q.bindValue(":id", m_examDataId);
		}

		q.exec();
		checkQuery(q);

		savedSuccessfully = q.isActive();
	}

	return savedSuccessfully;
}
