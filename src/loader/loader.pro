QT			+=	core gui sql
TARGET		 =	loader
TEMPLATE	 =	app

include( ../m-system-config.pri )

LIBS		=  -lcryptopp -L$${ROOTPATH}/bin/


SOURCES		+=	main.cpp\
				mainwindow.cpp \
				logindialog.cpp \
				settingsdialog.cpp \
				databasesettingspage.cpp \
				tabwidget.cpp \
				homepage.cpp \
				aboutdialog.cpp \
				database.cpp \
				user.cpp \
				demo.cpp

HEADERS		+=	mainwindow.h \
				logindialog.h \
				settingsdialog.h \
				abstractsettingspage.h \
				databasesettingspage.h \
				tabwidget.h \
				homepage.h \
				aboutdialog.h \
				database.h \
				user.h \
				demo.h

FORMS		+=	mainwindow.ui \
				logindialog.ui \
				settingsdialog.ui \
				databasesettingspage.ui

RESOURCES	+=	$${ROOTPATH}/src/resources/resources.qrc
