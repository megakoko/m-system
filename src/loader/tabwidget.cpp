

#include "tabwidget.h"
#include <QTabBar>
#include <QMessageBox>
#include <QDebug>
#include "saveablepluginwidget.h"


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

	SaveablePluginWidget* saveableWidget = dynamic_cast<SaveablePluginWidget*>(w);
	if(saveableWidget != NULL)
	{
		QString errorDescription;

		if(saveableWidget->canSave(errorDescription))
		{
			// Убеждаемся, что по-ошибке переменная не была использована.
			Q_ASSERT(errorDescription.isNull());

			if(userWantsToSave())
				saveableWidget->save();
		}
		else
		{
			const QString& title = QString::fromUtf8("Сохранение невозможно");
			const QString& description = errorDescription + "\n\n" +
										QString::fromUtf8("Все-равно закрыть?");

			const int rc = QMessageBox::question(this, title, description,
												 QMessageBox::Yes | QMessageBox::No);

			if(rc == QMessageBox::No)
				return;
		}
	}

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


bool TabWidget::userWantsToSave()
{
	const QString& title = QString::fromUtf8("Сохранение");
	const QString& descr = QString::fromUtf8("Хотите сохранить?");

	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);

	return (rc == QMessageBox::Yes);
}
