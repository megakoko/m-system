QT			+= core gui

TARGET		= msystem_dummy_db
TEMPLATE	= app


BASEPATH = ../

DESTDIR		=	$${BASEPATH}/bin/
OBJECTS_DIR =	$${BASEPATH}/bin/objects
MOC_DIR		=	$${BASEPATH}/bin/moc
UI_DIR		=	$${BASEPATH}/bin/ui
RCC_DIR		=	$${BASEPATH}/bin/rcc




SOURCES +=	main.cpp \
			mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
