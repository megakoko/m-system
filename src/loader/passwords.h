
#pragma once

#include <QString>

namespace Passwords
{
	QString hash(const QString& password, const QByteArray& salt);
	QString encodePassword(const QString& pw);
	QString decodePassword(const QString& pw);
	QByteArray salt();
}
