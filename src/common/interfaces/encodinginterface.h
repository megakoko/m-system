#pragma once

class QString;


/// Интерфейс для использования шифрования в программе.
class EncodingInterface
{
public:
	/// Шифрует строку \a plainText и возвращает результат шифрования.
	virtual QString encode(const QString& plainText) const = 0;
	/// Дешифрует строку \a cipherText и возвращает результат дешифрования.
	virtual QString decode(const QString& cipherText) const = 0;

	/// Возвращает MD5-хэш от MD5-хэша строки \a plainText, посоленной солью \a salt.
	virtual QString password(const QString& plainText, const QString& salt) const = 0;

	/// Возвращает соль (строку из случайных символов) длины \a length.
	virtual QString salt(const int length) const = 0;

};
