#pragma once

#include "ui_maintherapeutistwidget.h"
#include "pluginwidget.h"

class MainTherapeutistWidget : public PluginWidget, private Ui::MainTherapeutistWidget
{
	Q_OBJECT

public:
	explicit MainTherapeutistWidget(QWidget* parent = 0);

};
