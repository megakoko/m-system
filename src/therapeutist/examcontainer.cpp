#include "examcontainer.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


static const int labelColumn = 0;
static const int widgetColumn = 1;
static const int resetButtonColumn = 2;


ExamContainer::ExamContainer(const int examId, const QString &textid,
							 const QString &labelText, const bool topLevel)
	: ExamWidget(examId, textid, labelText)
	, m_topLevel(topLevel)
	, m_widget(new QWidget())
	, m_headerIndicator(NULL)
	, m_headerText(NULL)
	, m_container(new QWidget())
{
	static const int indicatorWidth = 20;
	QHBoxLayout* header = NULL;

	// Чтобы код лучше читался.
	const bool needHeader = !topLevel;


	// Создаем заголовок.
	if(needHeader)
	{
		m_headerIndicator = new QLabel;
		m_headerText = new QLabel;

		m_headerIndicator->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		m_headerIndicator->setMinimumWidth(indicatorWidth);

		header = new QHBoxLayout;
		header->setContentsMargins(QMargins());
		header->addWidget(m_headerIndicator, 0, Qt::AlignLeft);
		header->addWidget(m_headerText, 0, Qt::AlignLeft);
		header->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
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
		margins.setLeft(indicatorWidth + header->spacing());

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
}


void ExamContainer::init()
{

}


QLabel* ExamContainer::label() const
{
	return NULL;
}


QWidget* ExamContainer::widget() const
{
	return m_widget;
}


void ExamContainer::updateHeader()
{
	if(!m_topLevel)
	{
		const bool childrenValuesAreNull = valueIsNull();
		const bool containerIsExpanded = m_container->isVisible();

		const QString& link	=
				QString("<a style='font-weight: %1; color: black; text-decoration: none' href='ref'>%2</a>")
				.arg(childrenValuesAreNull ? "normal" : "bold")
				.arg(m_labelText);

		QString text = link;
		if(!childrenValuesAreNull && !containerIsExpanded)
			text.append("<br>" + value());

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
		q.prepare("SELECT textid FROM UiElement WHERE parentId = ? ORDER BY id");
		q.addBindValue(m_textid);
		q.exec();

		while(q.next())
		{
			ExamWidget* widget = m_factory.createWidget(m_examId, q.value(0).toString());

			if(widget != NULL && widget->widget() != NULL)
			{
				connect(widget, SIGNAL(valueChanged(bool)), SLOT(updateHeader()));
				connect(widget, SIGNAL(valueChanged(bool)), SIGNAL(valueChanged(bool)));

				const int row = m_items.count();
				m_items.append(widget);

				if(widget->label() == NULL)
				{
					// Виджет будет занимать нулевую и первую колонку.
					m_containerLayout->addWidget(widget->widget(),
												 row, labelColumn,
												 1, labelColumn + 2);
				}
				else
				{
					widget->label()->setWordWrap(true);

					m_containerLayout->addWidget(widget->label(), row, labelColumn);
					m_containerLayout->addWidget(widget->widget(), row, widgetColumn);

					if(widget->valueCanBeReseted())
					{
						QPushButton* resetButton =
								new QPushButton(QIcon(":/cancel.png"), QString::null);
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


QString ExamContainer::value() const
{
	static const QString labelAndValueDelimiter = ": ";


	QStringList values;

	if(!m_items.isEmpty())
	{
		foreach(const ExamWidget* widget, m_items)
			if(!widget->valueIsNull())
				values.append(widget->labelText() + labelAndValueDelimiter + widget->value());
	}
	else if(m_examId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT ui.label, d.textValue, d.integerValue, uiEnum.value "
				  " FROM ExaminationData d "
				  " LEFT JOIN UiElement ui ON d.uielementId = ui.id "
				  " LEFT JOIN UiElementEnums uiEnum ON "
					" (ui.textid = uiEnum.uiElementTextId AND d.enumValue = uiEnum.id) "
				  " WHERE d.examinationId = ? AND ui.parentId = ? "
				  " ORDER BY ui.id ");

		q.addBindValue(m_examId);
		q.addBindValue(m_textid);
		q.exec();
		checkQuery(q);


		while(q.next())
		{
			QString val;

			if(!q.value(1).isNull())
				val = q.value(1).toString();
			else if(!q.value(2).isNull())
				val = q.value(2).toString();
			else if(!q.value(3).isNull())
				val = q.value(3).toString();

			if(!val.isNull())
				values << q.value(0).toString() + labelAndValueDelimiter + val;
			else
			{
				qWarning() << "No value for examination data with;"
						   << "examId =" << m_examId
						   << "textId =" << m_textid;
			}
		}
	}

	return values.join("; ");
}


bool ExamContainer::save(const int examId) const
{
	bool savedSuccessfully = true;
	foreach(const ExamWidget* widget, m_items)
		savedSuccessfully &= widget->save(examId);

	return savedSuccessfully;
}
