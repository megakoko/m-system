#include "encoding.h"


#include <QDateTime>
#include <QDebug>


const QVector<QChar> Encoding::availableChars = Encoding::generateVector();

const QVector< QVector<QChar> > Encoding::vigenereTable = Encoding::generateTable();


QVector<QChar> Encoding::charactersBetween(const QString& from, const QString& to)
{
	Q_ASSERT(!from.isEmpty());
	Q_ASSERT(!to.isEmpty());

	QVector<QChar> result;
	for(int i = from.at(0).unicode(); i <= to.at(0).unicode(); ++i)
		result << QChar(i);

	if(result.isEmpty())
		qWarning() << "'from' is greater than 'to' in" << __FILE__ << __FUNCTION__;

	return result;
}


QVector<QChar> Encoding::generateVector()
{
	QVector<QChar> vector;

	// Специальные символы и цифры.
	vector << charactersBetween(QString::fromUtf8(" "), QString::fromUtf8("@"));

	// Между A-Z и a-z в Юникоде находятся ненужные нам символы.
	vector << charactersBetween(QString::fromUtf8("A"), QString::fromUtf8("Z"));
	vector << charactersBetween(QString::fromUtf8("a"), QString::fromUtf8("z"));

	// Кириллица.
	vector << charactersBetween(QString::fromUtf8("А"), QString::fromUtf8("я"));

	return vector;
}


QVector< QVector<QChar> > Encoding::generateTable()
{
	QVector< QVector<QChar> > table;

	table.resize(availableChars.size());
	for(int row = 0; row < availableChars.size(); ++row)
	{
		table[row].resize(availableChars.size());
		for(int col = 0; col < availableChars.size(); ++col)
			table[row][col] = availableChars.at((col+row) % availableChars.size());
	}


	// Вывод сгенерированной таблицы.
	/*
	for(int row = 0; row < vector.size(); ++row)
	{
		QString str;
		for(int col = 0; col < vector.size(); ++col)
			str += table[row][col];

		qDebug() << qPrintable(str);
	}
	*/

	return table;
}



QString Encoding::encode(const QString& input, const QString& code)
{
	Q_ASSERT(input.size() == code.size());

	QString result;
	for(int i = 0; i < input.size(); ++i)
	{
		const int charPosition = availableChars.indexOf(input.at(i));
		const int codePosition = availableChars.indexOf(code.at(i));

		Q_ASSERT(charPosition >= 0 && codePosition >= 0);

		result += vigenereTable[codePosition][charPosition];
	}

	return result;
}


QString Encoding::decode(const QString& input, const QString& code)
{
	Q_ASSERT(input.size() == code.size());

	QString result;
	for(int i = 0; i < input.size(); ++i)
	{
		const int codePosition = availableChars.indexOf(code.at(i));
		const int charPosition = vigenereTable[codePosition].indexOf(input.at(i));

		Q_ASSERT(charPosition >= 0 && codePosition >= 0);

		result += availableChars[charPosition];
	}

	return result;
}



/*
// TODO: сделать какой-нибудь автоматический тест, который проверял бы шифр.
QString generateString(const int length)
{
	static const QVector<QChar>& availableChars = generateVector();

	qsrand(QDateTime::currentMSecsSinceEpoch());

	QString string;
	for(int i = 0; i < length; ++i)
		string += availableChars[qrand() % availableChars.size()];

	return string;
}


QString addNoise(const QString& string)
{
	static const QVector<QChar>& availableChars = generateVector();
	QString newString(string);

	Q_ASSERT(!string.isEmpty());

	qsrand(QDateTime::currentMSecsSinceEpoch());

	for(int i = 0; i < string.size() / 10; ++i)
	{
		const int position = qrand() % string.size();
		const QChar& ch = availableChars[qrand() % availableChars.size()];

		newString[position] = ch;
	}

	Q_ASSERT(string != newString);

	return newString;
}

// Где-то внутри теста.
for(int length = 10; length < 100; ++length)
{
	const QString& input = generateString(length);
	const QString& inputWithNoise = addNoise(input);
	const QString& pass = generateString(length);

	const QString& decoded = decode(encode(input, pass), pass);

	if(decoded != input)
		qWarning() << "Decoded:" << decoded << ";\t expected:" << input;

	if(decoded == inputWithNoise)
		qWarning() << "Decoded:" << decoded << "; input with noise:" << inputWithNoise;
}

*/

QString Encoding::generateKeyword(const int length)
{
	// Случайный ключ.
	static const QString base = QString::fromUtf8("Ad4дqJ2ы");

	return base.repeated(1 + length/base.length()).left(length);
}


QString Encoding::encode(const QString& input) const
{
	Q_ASSERT(canUseEncoding(input));
	return encode(input, generateKeyword(input.length()));
}


QString Encoding::decode(const QString& input) const
{
	Q_ASSERT(canUseEncoding(input));
	return decode(input, generateKeyword(input.length()));
}


bool Encoding::canUseEncoding(const QString &input) const
{
	bool result = true;

	foreach(const QChar& ch, input)
		if(!availableChars.contains(ch))
		{
			result = false;
			break;
		}

	return result;
}
