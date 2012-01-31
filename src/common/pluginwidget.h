#pragma once
#include <QWidget>

/// Виджет модуля. Размещается во вкладке главного окна программы.
class PluginWidget : public QWidget
{
	Q_OBJECT
public:
	/// Конструктор.
	PluginWidget(QWidget* parent = 0)
		: QWidget(parent)
	{		}

	/// Деструктор.
	virtual ~PluginWidget() {}

signals:
	/// Сообщает программе, что надо добавить виджет \a widget в новой вкладке
	/// с названием \a caption.
	void addNewWidget(PluginWidget* widget, const QString& caption);

	/// Позволяет поменять название вкладки, в которой находится виджет, на \a caption.
	void setTabLabel(const QString& caption);

	/// Сообщает программе, что надо закрыть вкладку с данным виджетом.
	void closeMe();

};