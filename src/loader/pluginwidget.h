

#pragma once
#include <QWidget>

class PluginWidget : public QWidget
{
	Q_OBJECT
public:
	PluginWidget(QWidget* parent = 0)
		: QWidget(parent)
	{		}
	virtual ~PluginWidget() {}

signals:
	void addNewWidget(PluginWidget* widget, const QString& caption);
	void closeMe();

};
