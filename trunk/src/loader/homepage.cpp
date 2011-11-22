

#include "homepage.h"
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>


HomePage::HomePage(QWidget *parent)
	: QWidget(parent)
	, m_pluginGrid(new QGridLayout(this))
{
	setLayout(m_pluginGrid);
}


void HomePage::addButton(const QString &text, const QString& textid)
{
	static const int buttonMaxWidth = 200;

	const int cols = 3;
	const int count = m_pluginGrid->count();

	QPushButton* btn = new QPushButton(text);
	connect(btn, SIGNAL(clicked()), SLOT(buttonClicked()));
	btn->setMaximumWidth(buttonMaxWidth);
	m_pluginGrid->addWidget(btn, count/cols, count % cols);

	m_btnToTextid[btn] = textid;
}


void HomePage::clearButtons()
{
	QLayoutItem *item;
	while ((item = m_pluginGrid->takeAt(0)) != 0)
	{
		QWidget* w = item->widget();

		delete item;
		delete w;
	}
	m_btnToTextid.clear();
}


void HomePage::buttonClicked()
{
	QObject* sndr = sender();
	if(m_btnToTextid.contains(sndr))
		emit buttonClicked(m_btnToTextid.value(sndr));
}
