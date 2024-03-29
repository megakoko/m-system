#include "examcombobox.h"

#include <QComboBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


ExamComboBox::ExamComboBox(const int examId, const QString &textId)
	: ExamInputWidget(examId, textId)
	, m_comboBox(new QComboBox())
{
	m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
	connect(m_comboBox, SIGNAL(currentIndexChanged(int)), SLOT(comboBoxIndexChanged()));


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


QWidget* ExamComboBox::widget() const
{
	return m_comboBox;
}


bool ExamComboBox::valueIsNull() const
{
	return (m_comboBox->currentIndex() == -1);
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
		emit valueChanged(valueIsNull());
	}
}


void ExamComboBox::comboBoxIndexChanged()
{
	emit valueChanged(valueIsNull());
}


void ExamComboBox::init()
{
	const QVariant& value = initHelpher("enumValue");

	if(!value.isNull())
		m_comboBox->setCurrentIndex(m_comboBox->findData(value));
}


bool ExamComboBox::save(const int examId)
{
	const QVariant& value = m_comboBox->itemData(m_comboBox->currentIndex());
	return saveHelper("enumValue", examId, value);
}


QMap<int, QVariant> ExamComboBox::data() const
{
	QMap<int, QVariant> result;
	if(!valueIsNull())
		result[m_uielementId] = m_comboBox->itemData(m_comboBox->currentIndex());
	return result;
}
