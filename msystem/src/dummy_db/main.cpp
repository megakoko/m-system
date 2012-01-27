#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setOrganizationName("Andrew_Chukavin");
	a.setApplicationName("M-System");

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	MainWindow w;
	w.show();

	return a.exec();
}
