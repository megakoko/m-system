#pragma once

class QString;
class QDate;


/// Интерфейс для использования шифрования в программе.
class EncodingInterface
{
public:
	/// Деструктор.
	virtual ~EncodingInterface() {}


	/// Шифрует строку \a plainText и возвращает результат шифрования.
	virtual QString encodeStr(const QString& plainText) const = 0;
	/// Дешифрует строку \a cipherText и возвращает результат дешифрования.
	virtual QString decodeStr(const QString& cipherText) const = 0;


	/// Шифрует дату \a plainDate и возвращает результат шифрования.
	virtual QString encodeDate(const QDate& plainDate) const = 0;
	/// Дешифрует дату \a cipherDate и возвращает результат дешифрования.
	virtual QDate decodeDate(const QString& cipherDate) const = 0;


	/// Возвращает MD5-хэш от MD5-хэша строки \a plainText, посоленной солью \a salt.
	virtual QString password(const QString& plainText, const QString& salt) const = 0;

	/// Возвращает соль (строку из случайных символов) длины \a length.
	virtual QString salt(const int length) const = 0;

};
