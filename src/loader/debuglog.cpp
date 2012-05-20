#include "debuglog.h"

#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QSettings>
#include <QDateTime>


namespace DebugLog
{


QString filename = QSettings().value("debugLogFilename", "log.txt").toString();


void messageHandler(QtMsgType type, const char *msg)
{
	QString txt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss \t");
	switch (type) {
	case QtDebugMsg:
		txt += QString("Debug: %1").arg(msg);
		break;
	case QtWarningMsg:
		txt += QString("Warning: %1").arg(msg);
		break;
	case QtCriticalMsg:
		txt += QString("Critical: %1").arg(msg);
		break;
	case QtFatalMsg:
		txt += QString("Fatal: %1").arg(msg);
		abort();
	}

	QFile logFile(filename);
	if(logFile.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream out(&logFile);
		out << txt << endl;
	}
}


void setFilename(const QString &f)
{
	filename = f;
}

}
