
#pragma once

#include <QString>

namespace Passwords
{
	QString hash(const QString& password, const QByteArray& salt);
	QByteArray salt();
}
