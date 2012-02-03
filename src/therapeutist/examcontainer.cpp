#include "examcontainer.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


ExamContainer::ExamContainer(const int examId, const QString &textid, const QString &label)
	: ExamWidget(examId, textid, label)
	, m_header(new QPushButton(label, this))
	, m_container(new QWidget(this))
	, m_footer(new QLabel(this))
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_header);
	layout->addWidget(m_container);
	layout->addWidget(m_footer);
	setLayout(layout);

	QVBoxLayout* containerLayout = new QVBoxLayout;
	m_container->setLayout(containerLayout);

	if(label.isNull())
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

			if(widget != NULL)
			{
				m_items.append(widget);
				m_container->layout()->addWidget(widget);
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
		values.append(widget->label() + ": " + widget->value());

	return values.join("; ");
}


bool ExamContainer::save(const int examId) const
{
	bool savedSuccessfully = true;
	foreach(const ExamWidget* widget, m_items)
		savedSuccessfully &= widget->save(examId);

	return savedSuccessfully;
}
