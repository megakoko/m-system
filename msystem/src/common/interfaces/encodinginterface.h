#pragma once

#include <QString>



/// Интерфейс для использования шифрования в программе.
class EncodingInterface
{
public:
	/// Шифрует строку \a input и возвращает результат шифрования.
	virtual QString encode(const QString& input) const = 0;
	/// Дешифрует строку \a input и возвращает результат дешифрования.
	virtual QString decode(const QString& input) const = 0;

	/// Возвращает true, если возможно использование шифра для строки \a input.
	virtual bool canUseEncoding(const QString& input) const = 0;
};
