#include "ruleitemeditdialog.h"

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"
#include "symptompickerdialog.h"


namespace ValuePage {
	enum {
		textValuePage,
		realValuePage,
		enumValuePage
	};
}


RuleItemEditDialog::RuleItemEditDialog(const RuleItem &ruleItem, QWidget *parent)
	: QDialog(parent)
	, m_ruleItem(ruleItem)
{
	setupUi(this);

	init();
	initConnections();
}


void RuleItemEditDialog::init()
{
	updateSymptomNameAndValueWidgets();

	switch(m_stackedWidget->currentIndex())
	{
	case ValuePage::textValuePage:
		m_textValue->setText(m_ruleItem.textValue().toString());
		break;
	case ValuePage::realValuePage:
		m_realValue->setValue(m_ruleItem.realValue().toDouble());
		if(currentOperatorHasTwoValues())
			m_realValue2->setValue(m_ruleItem.realValue2().toDouble());
		break;
	case ValuePage::enumValuePage:
		m_enumValue->setCurrentIndex(m_enumValue->findData(m_ruleItem.enumValue()));
		break;
	default:
		Q_ASSERT(false);
	}


	// Переводим из вероятности [0;1] в промилле.
	m_probabilityWithDisease->setValue(1000.0 * m_ruleItem.probabilityWithDisease());
	m_probabilityWithoutDisease->setValue(1000.0 * m_ruleItem.probabilityWithoutDisease());

	checkFields();
	operatorChanged();
}


void RuleItemEditDialog::initConnections()
{
	connect(m_chooseSymptom, SIGNAL(clicked()), SLOT(chooseSymptom()));
	connect(m_operator, SIGNAL(currentIndexChanged(int)), SLOT(operatorChanged()));

	connect(m_operator, SIGNAL(currentIndexChanged(int)), SLOT(checkFields()));
	connect(m_textValue, SIGNAL(textEdited(QString)), SLOT(checkFields()));
	connect(m_enumValue, SIGNAL(currentIndexChanged(int)), SLOT(checkFields()));

	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));
}


void RuleItemEditDialog::updateSymptomNameAndValueWidgets()
{
	if(m_ruleItem.uiElementId() != 0)
	{
		QSqlQuery q;
		q.prepare("SELECT textid, label, typeid FROM UiElement WHERE id = ?");
		q.addBindValue(m_ruleItem.uiElementId());
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			m_symptomName->setText(q.value(1).toString());

			const QString& type = q.value(2).toString();
			if(type == "combobox")
			{
				populateOperatorList(QStringList() << "equal" << "notEqual");

				const QVariant& textid = q.value(0);

				q.prepare(" SELECT id, value FROM UiElementEnums "
						  " WHERE uiElementTextId = ? ORDER BY id");
				q.addBindValue(textid);
				q.exec();
				checkQuery(q);

				m_enumValue->clear();
				while(q.next())
					m_enumValue->addItem(q.value(1).toString(), q.value(0));


				m_stackedWidget->setCurrentIndex(ValuePage::enumValuePage);
			}
			else if(type == "spinbox" || type == "age")
			{
				populateOperatorList(QStringList() << "equal" << "notEqual" << "more" << "less" << "between");

				m_stackedWidget->setCurrentIndex(ValuePage::realValuePage);
			}
			else
			{
				populateOperatorList(QStringList() << "equal");

				m_stackedWidget->setCurrentIndex(ValuePage::textValuePage);
			}
		}
	}
	else
	{
		m_stackedWidget->setCurrentIndex(ValuePage::textValuePage);
	}

	m_operator->setCurrentIndex(m_operator->findData(m_ruleItem.operatorId()));
}


void RuleItemEditDialog::populateOperatorList(const QStringList &operatorTextids)
{
	QSqlQuery q;
	q.prepare("SELECT description, sign, id FROM Operator WHERE textid = ?");

	m_operator->clear();
	foreach(const QString& textid, operatorTextids)
	{
		q.addBindValue(textid);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			const QString& itemName = QString("%1 (%2)").
									  arg(q.value(0).toString()).
									  arg(q.value(1).toString());
			m_operator->addItem(itemName, q.value(2));
		}
		else
			qWarning() << "No operator with textid =" << textid;
	}
}


void RuleItemEditDialog::chooseSymptom()
{
	SymptomPickerDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_ruleItem.setUiElementId(d.selectedSymptom());
		updateSymptomNameAndValueWidgets();
		checkFields();
	}
}


int betweenOperatorId()
{
	QSqlQuery q;
	q.prepare("SELECT id FROM Operator WHERE textid = ?");
	q.addBindValue("between");
	q.exec();
	checkQuery(q);

	const bool hasRecord = q.first();
	Q_ASSERT(hasRecord); Q_UNUSED(hasRecord);

	return q.value(0).toInt();
}


bool RuleItemEditDialog::currentOperatorHasTwoValues() const
{
	static const int betweenId = betweenOperatorId();
	const QVariant& currentOperatorId = m_operator->itemData(m_operator->currentIndex());

	return currentOperatorId.toInt() == betweenId;
}


void RuleItemEditDialog::operatorChanged()
{
	if(m_stackedWidget->currentIndex() == ValuePage::realValuePage)
		m_realValue2->setVisible(currentOperatorHasTwoValues());
}


void RuleItemEditDialog::checkFields()
{
	bool buttonShouldBeEnabled = false;
	if(m_ruleItem.uiElementId() != 0 && m_operator->currentIndex() != -1)
	{
		switch(m_stackedWidget->currentIndex())
		{
		case ValuePage::enumValuePage:
			buttonShouldBeEnabled = m_enumValue->currentIndex() != -1;
			break;
		case ValuePage::realValuePage:
			buttonShouldBeEnabled = true;
			break;
		case ValuePage::textValuePage:
			buttonShouldBeEnabled = !m_textValue->text().isEmpty();
			break;
		default:
			Q_ASSERT(false);
		}
	}

	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(buttonShouldBeEnabled);
}


RuleItem RuleItemEditDialog::ruleItem()
{
	// Переводим из промилле в вероятность [0;1].
	m_ruleItem.setProbabilityWithDisease(m_probabilityWithDisease->value() / 1000.0);
	m_ruleItem.setProbabilityWithoutDisease(m_probabilityWithoutDisease->value() / 1000.0);

	m_ruleItem.setOperatorId(m_operator->itemData(m_operator->currentIndex()).toInt());

	switch(m_stackedWidget->currentIndex())
	{
	case ValuePage::realValuePage:
		if(currentOperatorHasTwoValues())
		{
			const double& value1 = m_realValue->value();
			const double& value2 = m_realValue2->value();

			m_ruleItem.setRealValueRange(qMin(value1, value2), qMax(value1, value2));
		}
		else
			m_ruleItem.setRealValue(m_realValue->value());
		break;
	case ValuePage::textValuePage:
		m_ruleItem.setTextValue(m_textValue->text());
		break;
	case ValuePage::enumValuePage:
		m_ruleItem.setEnumValue(m_enumValue->itemData(m_enumValue->currentIndex()).toInt());
		break;
	default:
		Q_ASSERT(false);
	}


	return m_ruleItem;
}
