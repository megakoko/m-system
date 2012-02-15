
#pragma once
#include "interfaces/interfaces.h"

class QString;
class PluginWidget;


/// Интерфейс модуля (плагина).
class PluginInterface {
public:
	/// Деструктор.
	virtual ~PluginInterface() {}

	/// Возвращает текстовый идентификатор textid модуля.
	/// Такой же идентификатор должен присутствовать в таблице Plugins базы данных.
	virtual QString textid() const = 0;
	/// Возвращает главный виджет модуля.
	virtual PluginWidget* widget() const = 0;

	/// Метод, позволяющий использовать другие интерфейсы. Они используются для связи
	/// модулей с основной программой. Для этого необходимо переопределить этот метод
	/// для "сохранения" где-нибудь указателя на интерфейсы \a interfaces.
	virtual void setInterfaces(const InterfacesPtr& interfaces) {
		Q_UNUSED(interfaces);
	}

};

Q_DECLARE_INTERFACE(PluginInterface, "com.msystem.Application.PluginInterface/1.0")
