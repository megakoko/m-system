#include "wrapper.h"

#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>

#include "modes.h"
#include "base64.h"


/*
	Для использования того или иного шифра надо раскомментировать define одного из них.
*/

//#define CIPHER_AES
#define CIPHER_GOST


/*
	Далее следуют два массива, необходимые для работы алгоритмов. Эти массивы
	сгенерированы случайно с помощью скрипта src/resources/randomhexarray.py. Для работы
	скрипта нужен Python, длину массива надо передавать в качестве параметра скрипта.

	key -- ключ шифра. Длина для ГОСТа -- 32 байта, для AES -- 16 байт.
	iv -- вектор инициализации. Длина для ГОСТа - 8 байт, для AES -- 16 байт.
*/


#ifdef CIPHER_AES
#include "aes.h"
#define CIPHER AES
byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH] = {
		0x23, 0x22, 0xD4, 0xA3,
		0x37, 0xE9, 0xD8, 0xB9,
		0xB5, 0xEC, 0xFF, 0x17,
		0x4A, 0x54, 0x93, 0x7A
};

byte iv[CryptoPP::CIPHER::BLOCKSIZE] = {
		0xCA, 0x7E, 0x86, 0x38,
		0xA4, 0x45, 0xDD, 0xA7,
		0x50, 0x17, 0xBA, 0x41,
		0x5D, 0xAE, 0x36, 0xBD
};
#endif


#ifdef CIPHER_GOST
#include "gost.h"
#define CIPHER GOST

byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH] = {
	0x23, 0x22, 0xD4, 0xA3,
	0x37, 0xE9, 0xD8, 0xB9,
	0xB5, 0xEC, 0xFF, 0x17,
	0x4A, 0x54, 0x93, 0x7A,
	0xDF, 0xE2, 0x09, 0x57,
	0x6B, 0x0A, 0x0D, 0xAB,
	0x7E, 0x54, 0x96, 0xDC,
	0x38, 0x91, 0xA0, 0x17
};

byte iv[CryptoPP::CIPHER::BLOCKSIZE] = {
	0xCA, 0x7E, 0x86, 0x38,
	0xA4, 0x45, 0xDD, 0xA7
};
#endif



QByteArray CryptoppWrapper::encodeByteArray(const QByteArray& plainBytes) const
{
	const QByteArray& inputArray(plainBytes);
	std::string cipherText;


	CryptoPP::CIPHER::Encryption encryption(key, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(encryption, iv);


	CryptoPP::StringSink* sink = new CryptoPP::StringSink(cipherText);
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, sink);


	const byte* bytes = reinterpret_cast<const byte*>(inputArray.data());
	stfEncryptor.Put(bytes, inputArray.length() + 1);
	stfEncryptor.MessageEnd();


	return QByteArray(cipherText.c_str(), cipherText.length()).toBase64();
}


QByteArray CryptoppWrapper::decodeByteArray(const QByteArray& cipherBytes) const
{
	QByteArray cipherByteArray = QByteArray::fromBase64(cipherBytes);
	std::string outputBytes;


	CryptoPP::CIPHER::Decryption decryption(key, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(decryption, iv);


	CryptoPP::StringSink* sink = new CryptoPP::StringSink(outputBytes);
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, sink);


	byte* bytes = reinterpret_cast<byte*>(cipherByteArray.data());
	stfDecryptor.Put(bytes, cipherByteArray.length());
	stfDecryptor.MessageEnd();


	return QByteArray(outputBytes.c_str(), outputBytes.length());
}


QString CryptoppWrapper::encodeStr(const QString& plainText) const
{
	QString encoded;

	try
	{
		encoded = QString(encodeByteArray(plainText.toUtf8()));
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to encode" << plainText << endl << "Reason:" << e.what();
	}

	return encoded;
}


QString CryptoppWrapper::decodeStr(const QString& cipherText) const
{
	QString decoded;

	try
	{
		decoded = QString(decodeByteArray(cipherText.toUtf8()));
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to decode" << cipherText << endl << "Reason:" << e.what();
	}

	return decoded;
}


QString CryptoppWrapper::encodeDate(const QDate& plainDate) const
{
	QString encoded;

	try
	{
		QByteArray arr;

		QDataStream stream(&arr, QIODevice::WriteOnly);
		stream << plainDate;

		encoded = QString(encodeByteArray(arr));
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to encode date" << plainDate << endl << "Reason:" << e.what();
	}

	return encoded;
}


QDate CryptoppWrapper::decodeDate(const QString &cipherDate) const
{
	QDate decoded;

	try
	{
		QByteArray arr = decodeByteArray(cipherDate.toUtf8());
		QDataStream stream(&arr, QIODevice::ReadOnly);

		stream >> decoded;
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to decode date" << cipherDate << endl << "Reason:" << e.what();
	}

	return decoded;
}


QString CryptoppWrapper::password(const QString &plainText, const QString &salt) const
{
	QCryptographicHash crypt(QCryptographicHash::Md5);
	crypt.addData(plainText.toLatin1(), plainText.length());
	const QByteArray& hash1 = crypt.result();

	crypt.reset();

	crypt.addData(hash1.toHex());
	crypt.addData(salt.toUtf8());
	const QByteArray& hash2 = crypt.result();

	return QString(hash2.toHex());
}


QString CryptoppWrapper::salt(const int length) const
{
	qsrand(QDateTime::currentMSecsSinceEpoch());

	QString result;
	for(int i = 0; i < length; ++i)
	{
		// 0x0030 = unicode "0".
		// "1", ..., "9", ":", ..., "@", "A", ...,"z"
		// 0x007A = unicode "z".
		const int start = 0x0030;
		const int end   = 0x007A;
		const int code = start + qrand() % (end - start);

		result.append(QChar(code));
	}

	return result;
}
