#pragma once

#include <QString>
#include "encodinginterface.h"


/*
	Реализация простейшего шифра -- шифра Виженера.
	( http://ru.wikipedia.org/wiki/Шифр_Виженера )
*/
class Encoding : public EncodingInterface
{
public:
	QString encode(const QString& pw) const;
	QString decode(const QString& pw) const;

	bool canUseEncoding(const QString &input) const;

	QString password(const QString &plainText, const QString &salt) const;

	QString salt(const int length) const;

private:
	static const QVector<QChar> availableChars;
	static const QVector< QVector<QChar> > vigenereTable;

	static QVector<QChar> charactersBetween(const QString& from, const QString& to);
	static QVector<QChar> generateVector();
	static QVector< QVector<QChar> > generateTable();
	static QString encode(const QString& input, const QString& code);
	static QString decode(const QString& input, const QString& code);

	static QString generateKeyword(const int length);
};
