

#include "tabwidget.h"

#include <QTabBar>
#include <QMessageBox>
#include <QVariant>
#include <QDebug>

#include "saveablepluginwidget.h"
#include "homepage.h"


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

	if(tabIsMain(index))
	{
		bool hasChildTabs = false;
		for(int i = 0; i < count(); ++i)
			if(tabsHaveSameTextid(index, i))
			{
				hasChildTabs = true;
				break;
			}


		if(hasChildTabs)
		{
			if(!userWantsToCloseMainPluginTab())
				return;
			else
			{
				for(int i = count()-1; i >= 0; --i)
					if(tabsHaveSameTextid(index, i))
					{
						QWidget* ww = widget(i);
						removeTab(i);
						delete ww;
					}
			}
		}
	}
	else
	{
		SaveablePluginWidget* saveableWidget = dynamic_cast<SaveablePluginWidget*>(w);
		if(saveableWidget != NULL)
		{
			QString errorDescription;

			if(saveableWidget->canSave(errorDescription))
			{
				// Убеждаемся, что по-ошибке переменная не была использована.
				Q_ASSERT(errorDescription.isNull());

				if(userWantsToSaveWidget())
					saveableWidget->save();
			}
			else
			{
				if(!userWantsToCloseWidgetThatCannotBeSaved(errorDescription))
					return;
			}
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


void TabWidget::setTabLabel(const QString &caption)
{
	if(sender() != NULL)
	{
		QWidget* w = qobject_cast<QWidget*>(sender());
		if(w != NULL)
		{
			const int index = indexOf(w);
			setTabText(index, caption);
			setTabToolTip(index, caption);
		}
	}
}


void TabWidget::addHomeTab(HomePage *homePage)
{
	Q_ASSERT(count() == 0);

	if(homePage != NULL)
	{
		const QString& caption = "Домашняя страница";
		const int index = addTab(homePage, caption);
		setTabToolTip(index, caption);
		setCurrentIndex(index);

	}
}


int TabWidget::addWidget(PluginWidget *widget, const QString &caption, const QString &textid)
{
	int index = -1;

	if(widget != NULL)
	{
		index = addTab(widget, caption);
		setTabToolTip(index, caption);
		setCurrentIndex(index);


		// Если textid равен значению по-умолчанию, добавляем в tabData() textid
		// вкладки, которая добавляет новую вкладку.
		if(textid.isNull())
		{
			const int parentIndex = indexOf(qobject_cast<QWidget*>(sender()));
			if(parentIndex >= 0)
			{
				const QStringList& data = tabBar()->tabData(parentIndex).toStringList();

				if(data.size() == 2)
					tabBar()->setTabData(index, data.last());
			}
		}
		// Если передали textid, значит добавляется главная вкладка модуля.
		// Добавляем в tabData() значения "main" и textid.
		else
		{
			const QStringList& data = QStringList() << "main" << textid;
			tabBar()->setTabData(index, data);
		}


		connect(widget, SIGNAL(closeMe()), SLOT(closeTab()));
		connect(widget, SIGNAL(setTabLabel(QString)), SLOT(setTabLabel(QString)));
		connect(widget,
				SIGNAL(addNewWidget(PluginWidget*, QString)),
				SLOT(addWidget(PluginWidget*,QString)));


	}
	return index;
}


bool TabWidget::tabIsMain(const int index) const
{
	const QStringList& tabData = tabBar()->tabData(index).toStringList();

	return !tabData.isEmpty() && tabData.first() == "main";
}


bool TabWidget::tabsHaveSameTextid(const int index1, const int index2) const
{
	bool textidsAreSame = false;

	if(index1 != index2)
	{
		const QStringList& data1 = tabBar()->tabData(index1).toStringList();
		const QStringList& data2 = tabBar()->tabData(index2).toStringList();

		textidsAreSame =!data1.isEmpty() &&
						!data2.isEmpty() &&
						(data1.last() == data2.last());
	}

	return textidsAreSame;
}


bool TabWidget::userWantsToCloseMainPluginTab()
{
	const QString& title = QString::fromUtf8("Закрытие модуля");
	const QString& descr = QString::fromUtf8("Хотите закрыть модуль и все его вкладки?");
	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);

	return rc == QMessageBox::Yes;
}


bool TabWidget::userWantsToSaveWidget()
{
	const QString& title = QString::fromUtf8("Сохранение");
	const QString& descr = QString::fromUtf8("Хотите сохранить?");

	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);

	return (rc == QMessageBox::Yes);
}


bool TabWidget::userWantsToCloseWidgetThatCannotBeSaved(const QString& errorDescription)
{
	const QString& title = QString::fromUtf8("Сохранение невозможно");
	const QString& description = errorDescription + "\n\n" +
								QString::fromUtf8("Все-равно закрыть?");

	const int rc = QMessageBox::question(this, title, description,
										 QMessageBox::Yes | QMessageBox::No);

	return rc == QMessageBox::Yes;
}
