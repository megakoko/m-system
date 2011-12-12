#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("Andrew_Chukavin");
	app.setApplicationName("M-System");

	MainWindow w;
	w.show();
	return app.exec();
}
