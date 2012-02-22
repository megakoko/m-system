#pragma once


/// Сохраняемый объект.
class SaveableObject
{
public:
	/// Может использоваться для обозначения "невозможного" идентификатора, или первичного
	/// ключа.
	static const int InvalidId = 0;

	/// Деструктор.
	virtual ~SaveableObject() { }

	/// Метод для сохранения содержимого виджета.
	virtual void save() = 0;

};
