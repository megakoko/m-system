#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("Andrew_Chukavin");
	app.setApplicationName("M-System");

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	MainWindow w;
	w.show();
	return app.exec();
}
