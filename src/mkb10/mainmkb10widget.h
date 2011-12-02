#pragma once

#include "pluginwidget.h"
#include "ui_mainmkb10widget.h"

class MainMkb10Widget : public PluginWidget, private Ui::MainMkb10Widget
{
    Q_OBJECT

public:
    explicit MainMkb10Widget(QWidget *parent = 0);
};
