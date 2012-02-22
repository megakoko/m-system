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

	if(!m_ruleItem.textValue().isNull())
		m_textValue->setText(m_ruleItem.textValue().toString());
	else if(!m_ruleItem.realValue().isNull())
		m_realValue->setValue(m_ruleItem.realValue().toDouble());
	else if(!m_ruleItem.enumValue().isNull())
		m_enumValue->setCurrentIndex(m_enumValue->findData(m_ruleItem.enumValue()));

	m_probabilityWithDisease->setValue(m_ruleItem.probabilityWithDisease());
	m_probabilityWithoutDisease->setValue(m_ruleItem.probabilityWithoutDisease());

	checkFields();
}


void RuleItemEditDialog::initConnections()
{
	connect(m_chooseSymptom, SIGNAL(clicked()), SLOT(chooseSymptom()));

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
				const QVariant& textid = q.value(0);

				q.prepare(" SELECT id, value FROM UiElementEnums "
						  " WHERE uiElementTextId = ? ");
				q.addBindValue(textid);
				q.exec();
				checkQuery(q);

				m_enumValue->clear();
				while(q.next())
					m_enumValue->addItem(q.value(1).toString(), q.value(0));


				m_stackedWidget->setCurrentIndex(ValuePage::enumValuePage);
			}
			else if(type == "spinbox")
			{
				m_stackedWidget->setCurrentIndex(ValuePage::realValuePage);
			}
			else
			{
				m_stackedWidget->setCurrentIndex(ValuePage::textValuePage);
			}
		}
	}
	else
	{
		m_stackedWidget->setCurrentIndex(ValuePage::textValuePage);
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


void RuleItemEditDialog::checkFields()
{
	bool buttonShouldBeEnabled = false;
	if(m_ruleItem.uiElementId() != 0)
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
	m_ruleItem.setProbabilityWithDisease(m_probabilityWithDisease->value());
	m_ruleItem.setProbabilityWithoutDisease(m_probabilityWithoutDisease->value());


	switch(m_stackedWidget->currentIndex())
	{
	case ValuePage::realValuePage:
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
