#pragma once

#include "pluginwidget.h"

/// Виджет модуля с возможностью сохранения данных. Эта возможность предоставляется
/// путем определения методов canSave() и save().
class SaveablePluginWidget : public PluginWidget
{
	Q_OBJECT
public:
	/// Конструктор.
	SaveablePluginWidget(QWidget* parent = 0)
		: PluginWidget(parent)
	{		}

	/// Может использоваться для обозначения "невозможного" идентификатора, или первичного
	/// ключа.
	static const int InvalidId = -1;

	/**
		Позволяет определить, можно ли сохранить данные, размещенные в виджете.
		Если сохранение невозможно, то описание причины этого сохраняется
		в \a errrorDescription.
		Метод вызывается после сигнала PluginWidget::closeMe(), либо после того, как
		пользователь щелкнул на кнопке [x] вкладки.

		Возвращает true если данные могут быть сохранены, иначе false.
	**/
	virtual bool canSave(QString& errorDescription) const = 0;

	/// Метод для сохранения содержимого виджета.
	virtual void save() = 0;

signals:
	/// Сообщает о том, что данные сохранены.
	void saved();

};
