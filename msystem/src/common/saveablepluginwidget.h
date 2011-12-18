#pragma once

#include "pluginwidget.h"

class SaveablePluginWidget : public PluginWidget
{
	Q_OBJECT
public:
	SaveablePluginWidget(QWidget* parent = 0)
		: PluginWidget(parent)
	{

	}

	/// Позволяет определить, можно ли сохранить данные, размещенные в виджете.
	/// Если сохранение невозможно, то описание причины этого сохраняется
	/// в \a errrorDescription.
	/// \returns	true если данные могут быть сохранены, иначе false.
	virtual bool canSave(QString& errorDescription) const = 0;
	virtual void save() = 0;

signals:
	void saved();

};
