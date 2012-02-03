#include "examcombobox.h"

#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


ExamComboBox::ExamComboBox(const int examId, const QString &textId, const QString &label)
	: ExamWidget(examId, textId, label)
	, m_label(new QLabel(label, this))
	, m_comboBox(new QComboBox(this))
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_label);
	layout->addWidget(m_comboBox);
	setLayout(layout);

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


bool ExamComboBox::valueIsNull() const
{
	return (m_comboBox->currentIndex() == -1);
}


QString ExamComboBox::value() const
{
	return m_comboBox->currentText();
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
			q.bindValue(":id", m_examDataId);
		}

		q.bindValue(":enumValue", m_comboBox->itemData(m_comboBox->currentIndex()));
		q.exec();
		checkQuery(q);

		savedSuccessfully = q.isActive();
	}

	return savedSuccessfully;
}
