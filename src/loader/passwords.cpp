

#include "passwords.h"
#include <QCryptographicHash>


QString Passwords::hash(const QString &password, const QByteArray &salt)
{

	QCryptographicHash crypt(QCryptographicHash::Md5);
	crypt.addData(password.toLatin1(), password.length());
	const QByteArray& hash1 = crypt.result();

	crypt.reset();

	crypt.addData(hash1.toHex());
	crypt.addData(salt);
	const QByteArray& hash2 = crypt.result();

	return hash2.toHex();
}

QString Passwords::encodePassword(const QString& pw)
{
	// TODO
	return pw;
}


QString Passwords::decodePassword(const QString& pw)
{
	// TODO
	return pw;
}


QByteArray Passwords::salt()
{
	const int length = 10;
	QByteArray result;
	for(int i = 0; i < length; ++i)
	{
		result.append('s');
	}

	return result;
}
