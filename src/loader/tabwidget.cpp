

#include "tabwidget.h"
#include <QTabBar>
#include <QDebug>
#include "pluginwidget.h"


TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
}


void TabWidget::tabInserted(int index)
{
	if(index == 0)
	{
		// Убираем кнопку закрытия [x] для первой вкладки.
		QWidget* button = new QWidget(this);
		button->resize(0, 0);
		tabBar()->setTabButton(index, QTabBar::RightSide, button);
	}

	QTabWidget::tabInserted(index);
}


void TabWidget::closeAllTabs()
{
	while(count() > 1)
		closeTab(1);
}


void TabWidget::closeTab(int index)
{
	QWidget* w = widget(index);
	removeTab(index);
	delete w;
}


void TabWidget::closeTab()
{
	const int index = indexOf(qobject_cast<QWidget*>(sender()));
	if(index >= 0)
		closeTab(index);
}


void TabWidget::setTabLabel(const QString &text)
{
	if(sender() != NULL)
	{
		QWidget* w = qobject_cast<QWidget*>(sender());
		if(w != NULL)
		{
			setTabText(indexOf(w), text);
		}
	}
}


void TabWidget::addWidget(PluginWidget *widget, const QString& caption)
{
	if(widget != NULL)
	{
		const int index = addTab(widget, caption);
		setCurrentIndex(index);

		connect(widget, SIGNAL(closeMe()), SLOT(closeTab()));
		connect(widget, SIGNAL(setTabLabel(QString)), SLOT(setTabLabel(QString)));
		connect(widget,
				SIGNAL(addNewWidget(PluginWidget*, QString)),
				SLOT(addWidget(PluginWidget*,QString)));
	}
}
