

#pragma once

#include <QTabWidget>

class PluginWidget;
class SaveablePluginWidget;
class HomePage;


class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	explicit TabWidget(QWidget *parent = 0);
	void closeAllTabs();

public slots:
	void closeTab(int index);
	void closeTab();

	void setTabLabel(const QString& text);

	void addHomeTab(HomePage* homePage);

	/// \returns номер добавленной вкладки или -1, если вкладка не была добавлена.
	int addWidget(PluginWidget* widget, const QString& caption);
	int addWidget(PluginWidget* widget, const QString& caption, const QString& textid);

protected:
	void tabInserted(int index);

private:
	bool tabIsMain(const int index) const;
	bool tabsHaveSameTextid(const int index1, const int index2) const;

	bool userWantsToCloseMainPluginTab();

	bool userWantsToSaveWidget();
	bool userWantsToCloseWidgetThatCannotBeSaved(const QString& errorDescription);

	/// Сохраняет виджет. Возвращает true, если виджет был сохранен.
	bool saveWidget(SaveablePluginWidget* widget);

private slots:
	bool saveSenderWidget();
	bool saveCurrentWidget();
	void closeCurrentTab();

	void onCurrentIndexChanged();

signals:
	void currentWidgetChanged(const bool widgetIsSaveable);

};
