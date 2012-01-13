#pragma once

class QString;


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

	/// Возвращает MD5-хэш от MD5-хэша строки \a plainText, посоленной солью \a salt.
	virtual QString password(const QString& plainText, const QString& salt) const = 0;

	/// Возвращает соль (строку из случайных символов) длины \a length.
	virtual QString salt(const int length) const = 0;

};
