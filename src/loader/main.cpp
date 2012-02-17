#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QFile>
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
	if(translator.load(":/qt_ru_4.7.3.qm"))
		app.installTranslator(&translator);
	else
		qCritical() << "Не найден файл перевода самой библиотеки Qt.";

	QFile f(":/stylesheet.css");
	if(f.open(QIODevice::ReadOnly))
		qApp->setStyleSheet(f.readAll());
	else
		qCritical() << "Не найден файл с QSS-стилями" << f.fileName();


	QLocale::setDefault(QLocale(QLocale::Russian, QLocale::RussianFederation));


	MainWindow w;
	w.show();
	return app.exec();
}
