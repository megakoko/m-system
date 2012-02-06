#include <QtGui/QApplication>
#include "lotusmainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LotusMainWindow w;
	w.show();
	
	return a.exec();
}
