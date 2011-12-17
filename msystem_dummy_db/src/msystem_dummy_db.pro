QT			+= core gui sql

TARGET		= msystem_dummy_db
TEMPLATE	= app


BASEPATH = ../

DESTDIR		=	$${BASEPATH}/bin/
OBJECTS_DIR =	$${BASEPATH}/bin/objects
MOC_DIR		=	$${BASEPATH}/bin/moc
UI_DIR		=	$${BASEPATH}/bin/ui
RCC_DIR		=	$${BASEPATH}/bin/rcc


RESOURCES+= resources/resources.qrc

SOURCES	 +=	main.cpp \
			mainwindow.cpp

HEADERS  += mainwindow.h \
			macros.h

FORMS    += mainwindow.ui
