#pragma once

#include <QString>
#include <QDate>
#include "interfaces/encodinginterface.h"


/*
	Шифр AES.
*/
class CryptoppWrapper : public EncodingInterface
{
public:
	QString encodeStr(const QString& plainText) const;
	QString decodeStr(const QString& cipherText) const;


	QString encodeDate(const QDate& plainDate) const;
	QDate decodeDate(const QString& cipherDate) const;


	QString password(const QString &plainText, const QString &salt) const;

	QString salt(const int length) const;

private:
	/// Шифрует \a plainBytes. Результат возвращает в Base64.
	QByteArray encodeByteArray(const QByteArray& plainBytes) const;

	/// Расшифровывает \a cipherBytes. Входной параметр должен быть в Base64.
	QByteArray decodeByteArray(const QByteArray& cipherBytes) const;

};
