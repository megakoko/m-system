#pragma once

#include <QString>
#include "encodinginterface.h"


/*
	Шифр AES.
*/
class CryptoppWrapper : public EncodingInterface
{
public:
	QString encode(const QString& plainText) const;
	QString decode(const QString& cipherText) const;

	QString password(const QString &plainText, const QString &salt) const;

	QString salt(const int length) const;

};
