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

		updateHeader();
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
		const QString& link	=
				QString("<a style='color: black; text-decoration: none' href='ref'>%1</a>")
				.arg(m_labelText);

		m_headerText->setText(link);
		m_headerIndicator->setText(m_container->isVisible() ? "-" : "+");
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
				}
			}
		}
	}

	m_container->setVisible(expanded);
	updateHeader();
}


bool ExamContainer::valueIsNull() const
{
	bool result = false;
	foreach(const ExamWidget* widget, m_items)
		if(widget->valueIsNull())
		{
			result = true;
			break;
		}
	return result;
}


QString ExamContainer::value() const
{
	QStringList values;
	foreach(const ExamWidget* widget, m_items)
		values.append(widget->labelText() + ": " + widget->value());

	return values.join("; ");
}


bool ExamContainer::save(const int examId) const
{
	bool savedSuccessfully = true;
	foreach(const ExamWidget* widget, m_items)
		savedSuccessfully &= widget->save(examId);

	return savedSuccessfully;
}
