#pragma once

#include "pluginwidget.h"
#include "saveableobject.h"

/// Виджет модуля с возможностью сохранения данных. Эта возможность предоставляется
/// путем определения методов canSave() и save().
class SaveablePluginWidget : public PluginWidget, public SaveableObject
{
	Q_OBJECT
public:
	/// Конструктор.
	SaveablePluginWidget(QWidget* parent = 0)
		: PluginWidget(parent)
	{		}

	/**
		Позволяет определить, можно ли сохранить данные, размещенные в виджете.
		Если сохранение невозможно, то описание причины этого сохраняется
		в \a errrorDescription.
		Метод вызывается программой перед закрытием вкладки с виджетом.

		Возвращает true если данные могут быть сохранены, иначе false.
	**/
	virtual bool canSave(QString& errorDescription) const = 0;

signals:
	/// Сообщает о том, что данные сохранены.
	void saved();

};
