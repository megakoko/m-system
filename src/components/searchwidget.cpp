#include "searchwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


SearchWidget::SearchWidget(QWidget *parent)
	: QWidget(parent)
	, m_label(new QLabel(this))
	, m_lineEdit(new QLineEdit(this))
	, m_searchButton(new QPushButton(this))
	, m_clearButton(new QPushButton(this))
{
	init();
	initConnections();
}


void SearchWidget::init()
{
	if(m_label->text().isEmpty())
		m_label->hide();


	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_label);
	layout->addWidget(m_lineEdit);
	layout->addWidget(m_clearButton);
	layout->addWidget(m_searchButton);
	setLayout(layout);
}


void SearchWidget::initConnections()
{
	connect(m_searchButton, SIGNAL(clicked()), SIGNAL(searchPressed()));
	connect(m_clearButton, SIGNAL(clicked()), m_lineEdit, SLOT(clear()));
	connect(m_clearButton, SIGNAL(clicked()), SIGNAL(searchPressed()));
	connect(m_lineEdit, SIGNAL(returnPressed()), SIGNAL(searchPressed()));
}


void SearchWidget::setLabelText(const QString &labelText)
{
	m_label->setText(labelText);
	m_label->setHidden(labelText.isEmpty());
}


QString SearchWidget::enteredText() const
{
	return m_lineEdit->text();
}
