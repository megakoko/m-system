#include "wrapper.h"

#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>

#include "modes.h"
#include "aes.h"
#include "base64.h"



// Ключ длинной в 16 байт. Сгенерирован случайно.
byte key[CryptoPP::AES::DEFAULT_KEYLENGTH] = {
	0x23, 0x22, 0xD4, 0xA3,
	0x37, 0xE9, 0xD8, 0xB9,
	0xB5, 0xEC, 0xFF, 0x17,
	0x4A, 0x54, 0x93, 0x7A
};


// Вектор инициализации длинной в 16 байт. Сгенерирован случайно.
byte iv[CryptoPP::AES::BLOCKSIZE] = {
	0xCA, 0x7E, 0x86, 0x38,
	0xA4, 0x45, 0xDD, 0xA7,
	0x50, 0x17, 0xBA, 0x41,
	0x5D, 0xAE, 0x36, 0xBD
};


QString CryptoppWrapper::encode(const QString& plainText) const
{
	QString result;
	try
	{
		const QByteArray& inputArray(plainText.toUtf8());
		std::string cipher;


		CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );


		CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipher ) );
		stfEncryptor.Put(reinterpret_cast<const byte*>(inputArray.data()), inputArray.length() + 1 );
		stfEncryptor.MessageEnd();


		result = QByteArray(cipher.c_str(), cipher.length()).toBase64();
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to encode" << plainText << endl << "Reason:" << e.what();
	}

	return result;
}


QString CryptoppWrapper::decode(const QString& cipherText) const
{
	QString result;
	try
	{
		QByteArray cipherByteArray = QByteArray::fromBase64(cipherText.toUtf8());
		std::string outputBytes;


		CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );


		CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( outputBytes ) );
		stfDecryptor.Put(reinterpret_cast<byte*>(cipherByteArray.data()), cipherByteArray.length());
		stfDecryptor.MessageEnd();


		result = QString::fromStdString(outputBytes.data());
	}
	catch(const CryptoPP::Exception& e)
	{
		qWarning() << "Failed to decode" << cipherText << endl << "Reason:" << e.what();
	}

	return result;
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
