

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
	connect(this, SIGNAL(currentChanged(int)), SLOT(onCurrentIndexChanged()));
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
	bool needToCloseTab = true;

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
				needToCloseTab = false;
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

				const int answer = askUserAboutSaving();
				if(answer == QMessageBox::Yes)
					saveableWidget->save();
				else if(answer == QMessageBox::Cancel)
					needToCloseTab = false;
			}
			else
			{
				if(!userWantsToCloseWidgetThatCannotBeSaved(errorDescription))
					needToCloseTab = false;
			}
		}
	}

	if(needToCloseTab)
	{
		removeTab(index);
		delete w;
	}
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
		const int index = addTab(homePage, QIcon(":/home.png"), QString::null);
		setTabToolTip(index, "Домашняя страница");
		setCurrentIndex(index);
	}
}


int TabWidget::addWidget(PluginWidget *widget, const QString &caption)
{
	int index = -1;

	if(widget != NULL)
	{
		index = addTab(widget, caption);
		setTabToolTip(index, caption);
		setCurrentIndex(index);


		const int parentIndex = indexOf(qobject_cast<QWidget*>(sender()));
		if(parentIndex >= 0)
		{
			const QStringList& data = tabBar()->tabData(parentIndex).toStringList();

			if(data.size() == 2)
				tabBar()->setTabData(index, data.last());
		}


		connect(widget, SIGNAL(requestToSetTabLabel(QString)), SLOT(setTabLabel(QString)));
		connect(widget,
				SIGNAL(requestToAddNewWidget(PluginWidget*, QString)),
				SLOT(addWidget(PluginWidget*,QString)));
	}
	return index;
}


int TabWidget::addWidget(PluginWidget *widget, const QString &caption,
						 const QString &textid)
{
	const int index = addWidget(widget, caption);

	if(index >= 0)
	{
		const QVariant& data = QStringList() << "main" << textid;
		tabBar()->setTabData(index, data);
	}
	return index;
}


void TabWidget::closeCurrentTab()
{
	const int index = currentIndex();
	closeTab(index);
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
	const QString& title = "Закрытие модуля";
	const QString& descr = "Хотите закрыть модуль и все его вкладки?";
	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);

	return rc == QMessageBox::Yes;
}


int TabWidget::askUserAboutSaving()
{
	const QString& title = "Закрытие вкладки";
	const QString& descr = "Хотите сохранить содержимое вкладки?";

	return QMessageBox::question(this, title, descr,
								 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
								 QMessageBox::Cancel);
}


bool TabWidget::userWantsToCloseWidgetThatCannotBeSaved(const QString& errorDescription)
{
	const QString& title = "Сохранение невозможно";
	const QString& description = errorDescription + "\n\n" + "Все-равно закрыть вкладку?";

	const int rc = QMessageBox::question(this, title, description,
										 QMessageBox::Yes | QMessageBox::No);

	return rc == QMessageBox::Yes;
}


void TabWidget::onCurrentIndexChanged()
{
	QWidget* widget = currentWidget();
	const bool currentWidgetIsSaveable = (dynamic_cast<SaveablePluginWidget*>(widget) != NULL);
	emit currentWidgetChanged(currentWidgetIsSaveable);
}
