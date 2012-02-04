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


ExamContainer::ExamContainer(const int examId, const QString &textid, const QString &labelText)
	: ExamWidget(examId, textid, labelText)
	, m_widget(new QWidget())
	, m_header(new QPushButton(labelText))
	, m_container(new QWidget())
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_header);
	layout->addWidget(m_container);
	m_widget->setLayout(layout);

	m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);


	m_containerLayout = new QGridLayout;
	QMargins margins = m_containerLayout->contentsMargins();
	margins.setRight(0);
	m_containerLayout->setContentsMargins(margins);
	m_containerLayout->setColumnStretch(labelColumn, 1);
	m_containerLayout->setColumnStretch(widgetColumn, 3);
	m_container->setLayout(m_containerLayout);
	m_container->setHidden(true);

	if(labelText.isNull())
		expandContainer(true);
	else
	{
		m_header->setCheckable(true);
		connect(m_header, SIGNAL(clicked(bool)), SLOT(expandContainer(bool)));
	}
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


void ExamContainer::expandContainer(bool expanded)
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
