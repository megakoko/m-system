#pragma once

#include <QString>


/*
	Реализация простейшего шифра -- шифра Виженера.
	( http://ru.wikipedia.org/wiki/Шифр_Виженера )
*/
namespace Encoding
{
	QString encodePassword(const QString& pw);
	QString decodePassword(const QString& pw);
}
