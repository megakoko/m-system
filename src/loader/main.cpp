#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("Andrew_Chukavin");
	app.setApplicationName("M-System");

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QTranslator translator;
	if(translator.load(":tr/qt_ru.qm"))
		app.installTranslator(&translator);
	else
		qCritical() << "Не найдет файл перевода самой библиотеки Qt.";

	MainWindow w;
	w.show();
	return app.exec();
}
