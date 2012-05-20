#pragma once

#include <QtGlobal>

namespace DebugLog
{
	void messageHandler(QtMsgType type, const char *msg);
	void setFilename(const QString& filename);

};

