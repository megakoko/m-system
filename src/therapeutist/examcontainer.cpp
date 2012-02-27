#include "examcontainer.h"

#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "therapeutist.h"
#include "examdiagnosisedit.h"
#include "macros.h"
#include "examinationeditwidget.h"


static const int spacerColumn = 0;
static const int labelColumn = 1;
static const int widgetColumn = 2;
static const int resetButtonColumn = 3;

const int ExamContainer::m_indicatorWidth = 20;

const QString ExamContainer::labelAndValueDelimiter = ": ";


ExamContainer::ExamContainer(const int examId, const QString &textid,
							 const bool topLevel)
	: ExamWidget(examId, textid)
	, m_topLevel(topLevel)
	, m_widget(new QWidget())
	, m_headerIndicator(NULL)
	, m_headerText(NULL)
	, m_container(new QWidget())
	, m_examinationEditWidget(NULL)
{
	QHBoxLayout* header = NULL;

	// Чтобы код лучше читался.
	const bool needHeader = !topLevel;


	// Создаем заголовок.
	if(needHeader)
	{
		m_headerIndicator = new QLabel;

		QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
		sizePolicy1.setHorizontalStretch(0);
		sizePolicy1.setVerticalStretch(0);
		sizePolicy1.setHeightForWidth(m_headerIndicator->sizePolicy().hasHeightForWidth());
		m_headerIndicator->setSizePolicy(sizePolicy1);

		m_headerIndicator->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
		m_headerIndicator->setMinimumWidth(m_indicatorWidth);


		m_headerText = new QLabel;
		m_headerText->setWordWrap(true);


		header = new QHBoxLayout;
		header->setContentsMargins(QMargins());
		header->addWidget(m_headerIndicator);
		header->addWidget(m_headerText);
	}


	// Создаем главный компоновщик.
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(QMargins());
	if(needHeader)
		layout->addLayout(header);
	layout->addWidget(m_container);
	m_widget->setLayout(layout);

	m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);


	// Создаем компоновщик для контейнера.
	m_containerLayout = new QGridLayout;
	QMargins margins = m_containerLayout->contentsMargins();
	margins.setRight(0);
	if(needHeader)
		margins.setLeft(m_indicatorWidth + header->spacing());

	m_containerLayout->setContentsMargins(margins);
	m_containerLayout->setColumnStretch(labelColumn, 1);
	m_containerLayout->setColumnStretch(widgetColumn, 3);
	m_container->setLayout(m_containerLayout);
	m_container->setHidden(true);


	//
	if(topLevel)
		expandContainer(true);
	else
		connect(m_headerText, SIGNAL(linkActivated(QString)), SLOT(expandContainer()));

	updateHeader();
	init();
}


void ExamContainer::init()
{
	if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT id FROM ExaminationData "
				  " WHERE examinationId = ? AND "
					" uiElementId = (SELECT id FROM UiElement WHERE textid = ?) ");
		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);

		if(q.first())
			m_examDataId = q.value(0).toInt();
	}
}


void ExamContainer::setExaminationEditWidget(ExaminationEditWidget *widget)
{
	m_examinationEditWidget = widget;
}


void ExamContainer::setLabelText(const QString &labelText, const QString &shortLabelText)
{
	ExamWidget::setLabelText(labelText, shortLabelText);
	updateHeader();
}


QLabel* ExamContainer::label() const
{
	return NULL;
}


QWidget* ExamContainer::widget() const
{
	return m_widget;
}


QMap<int, QVariant> ExamContainer::data() const
{
	QMap<int, QVariant> result;

	if(m_items.isEmpty())
		result = containerDataFromDatabase(m_textid);
	else foreach(const ExamWidget* widget, m_items)
		result.unite(widget->data());

	if(m_textid == "main" && m_examinationEditWidget != NULL)
	{
		QSqlQuery q("SELECT id FROM UiElement WHERE textid = 'age'");
		checkQuery(q);

		if(q.first())
		{
			const int uiElementId = q.value(0).toInt();
			const double age = m_examinationEditWidget->patientAge();

			result[uiElementId] = age;
		}
	}

	return result;
}


void ExamContainer::updateHeader()
{
	if(!m_topLevel)
	{
		const bool childrenValuesAreNull = valueIsNull();
		const bool containerIsExpanded = m_container->isVisible();

		const QString& link	=
				QString("<a style='font-weight: %1; color: black; text-decoration: none' "
						"href='ref'>%2</a>")
				.arg(childrenValuesAreNull ? "normal" : "bold")
				.arg(labelText());

		QString text = link;
		if(!childrenValuesAreNull && !containerIsExpanded)
			text.append("<br>" + value() + '.');

		m_headerText->setText(text);
		m_headerIndicator->setText(containerIsExpanded ? "-" : "+");
	}
}


void ExamContainer::expandContainer()
{
	expandContainer(!m_container->isVisible());
}


void ExamContainer::expandContainer(const bool expanded)
{
	if(m_items.isEmpty())
	{
		QSqlQuery q;
		q.prepare("SELECT id, textid FROM UiElement WHERE parentId = ? ORDER BY id");
		q.addBindValue(m_textid);
		q.exec();

		while(q.next())
		{
			const int uiElementId = q.value(0).toInt();
			const QString& uiElementTextId = q.value(1).toString();
			ExamWidget* widget = m_factory.createWidget(m_examId, uiElementTextId);

			if(widget != NULL && widget->widget() != NULL)
			{
				widget->setParent(this);
				widget->setUiElementId(uiElementId);

				connect(widget, SIGNAL(valueChanged(bool)), SLOT(updateHeader()));
				connect(widget, SIGNAL(valueChanged(bool)), SIGNAL(valueChanged(bool)));

				const int row = m_items.count();
				m_items.append(widget);

				if(widget->label() == NULL)
				{
					// Виджет будет занимать нулевую и первую колонку.
					m_containerLayout->addWidget(widget->widget(),
												 row, spacerColumn,
												 1, resetButtonColumn - spacerColumn + 1);
				}
				else
				{
					widget->label()->setWordWrap(true);

					QSpacerItem* spacer = new QSpacerItem(m_indicatorWidth, 0);
					m_containerLayout->addItem(spacer, row, spacerColumn);
					m_containerLayout->addWidget(widget->label(), row, labelColumn,
												 Qt::AlignTop);
					m_containerLayout->addWidget(widget->widget(), row, widgetColumn);

					if(widget->valueCanBeReseted())
					{
						QToolButton* resetButton = new QToolButton;
						resetButton->setProperty("type", "cancel");
						resetButton->setToolTip("Сбросить значение");
						resetButton->setDisabled(widget->valueIsNull());
						m_containerLayout->addWidget(resetButton, row, resetButtonColumn);


						connect(resetButton, SIGNAL(clicked()), widget, SLOT(resetValue()));

						connect(resetButton, SIGNAL(clicked(bool)),
								resetButton, SLOT(setEnabled(bool)));

						connect(widget, SIGNAL(valueChanged(bool)),
								resetButton, SLOT(setDisabled(bool)));
					}
				}


				if(uiElementTextId == "diagnosis" && m_textid == "main")
				{
					ExamDiagnosisEdit* diagnosis = dynamic_cast<ExamDiagnosisEdit*>(widget);
					if(diagnosis != NULL)
						diagnosis->setMainContainer(this);
				}
			}
		}
	}

	m_container->setVisible(expanded);
	updateHeader();
}


bool ExamContainer::valueIsNull() const
{
	bool result = true;

	if(!m_items.isEmpty())
	{
		foreach(const ExamWidget* widget, m_items)
			if(!widget->valueIsNull())
			{
				result = false;
				break;
			}
	}
	else if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT COUNT(*) FROM ExaminationData d "
				  " LEFT JOIN UiElement ui ON d.uielementId = ui.id "
				  " WHERE d.examinationId = ? AND ui.parentId = ? ");
		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);

		if(q.first())
			result = (q.value(0).toInt() == 0);
	}

	return result;
}


QStringList ExamContainer::containerValueFromDatabase(const QString& containerTextId) const
{
	QStringList values;

	QSqlQuery q;
	q.prepare(" SELECT ui.label, d.textValue, d.realValue, uiEnum.value, "
				" ui.typeid, ui.textId "
			  " FROM ExaminationData d "
			  " LEFT JOIN UiElement ui ON d.uielementId = ui.id "
			  " LEFT JOIN UiElementEnums uiEnum ON "
				" (ui.textid = uiEnum.uiElementTextId AND d.enumValue = uiEnum.id) "
			  " WHERE (d.examinationId = ? AND ui.parentId = ?) "
			  " ORDER BY ui.id ");

	q.addBindValue(m_examId);
	q.addBindValue(containerTextId);
	q.exec();
	checkQuery(q);


	while(q.next())
	{
		if(q.value(4).toString() == "container")
		{
			values << containerValueFromDatabase(q.value(5).toString());
		}
		else
		{
			QString val;
			val += q.value(0).toString() + labelAndValueDelimiter;


			bool recordHasValue = false;
			for(int recordColumn = 1; recordColumn <= 3; ++recordColumn)
				if(!q.value(recordColumn).isNull())
				{
					val += q.value(recordColumn).toString();
					recordHasValue = true;
					break;
				}
			Q_ASSERT(recordHasValue); Q_UNUSED(recordHasValue);


			values << val;
		}
	}

	return values;
}


QMap<int, QVariant>
ExamContainer::containerDataFromDatabase(const QString &containerTextId) const
{
	QMap<int, QVariant> result;

	QSqlQuery q;
	q.prepare(" SELECT ui.id, d.textValue, d.realValue, d.enumValue, ui.typeid, ui.textId "
			  " FROM ExaminationData d "
			  " LEFT JOIN UiElement ui ON d.uielementId = ui.id "
			  " WHERE d.examinationId = ? AND ui.parentId = ? "
			  " ORDER BY ui.id ");

	q.addBindValue(m_examId);
	q.addBindValue(containerTextId);
	q.exec();
	checkQuery(q);


	while(q.next())
	{
		if(q.value(4).toString() == "container")
		{
			result.unite(containerDataFromDatabase(q.value(5).toString()));
		}
		else
		{
			const int id = q.value(0).toInt();
			QVariant datum;
			for(int recordColumn = 1; recordColumn <= 3; ++recordColumn)
				if(!q.value(recordColumn).isNull())
				{
					datum = q.value(recordColumn);
					break;
				}

			if(!datum.isNull())
				result[id] = datum;
		}
	}

	return result;
}


QString ExamContainer::value() const
{
	QStringList values;

	if(!m_items.isEmpty())
	{
		foreach(const ExamWidget* widget, m_items)
			if(!widget->valueIsNull())
			{
				const bool widgetIsContainer =
						(dynamic_cast<const ExamContainer*>(widget) != NULL);

				if(widgetIsContainer)
					values << widget->value();
				else
				{
					values << widget->labelText() +
							  labelAndValueDelimiter +
							  widget->value();
				}
			}
	}
	else if(m_examId != InvalidId)
	{
		values = containerValueFromDatabase(m_textid);
	}

	return values.join(". ");
}


bool ExamContainer::save(const int examId)
{
	bool savedSuccessfully = true;
	foreach(ExamWidget* widget, m_items)
		savedSuccessfully &= widget->save(examId);


	if(m_examDataId != InvalidId || !valueIsNull())
	{
		QSqlQuery q;
		if(m_examDataId == InvalidId && !valueIsNull())
		{
			q.prepare(" INSERT INTO ExaminationData "
					  " (examinationId, uiElementId) "
					  " VALUES(:examId, (SELECT id FROM UiElement WHERE textid = :textid))"
					  + Therapeutist::interfaces->db->returningSentence("id"));
			q.bindValue(":examId", examId);
			q.bindValue(":textid", m_textid);

			q.exec();
			checkQuery(q);

			m_examDataId = Therapeutist::interfaces->db->lastInsertedId(&q).toInt();

			savedSuccessfully &= q.isActive();
		}
		else if(m_examDataId != InvalidId && valueIsNull())
		{
			q.prepare(" DELETE FROM ExaminationData WHERE id = :id");
			q.bindValue(":id", m_examDataId);

			q.exec();
			checkQuery(q);

			savedSuccessfully &= q.isActive();
		}
	}


	return savedSuccessfully;
}
