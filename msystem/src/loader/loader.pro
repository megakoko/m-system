QT			+=	core gui sql
TARGET		 =	loader
TEMPLATE	 =	app


include( ../m-system-config.pri )


SOURCES		+=	main.cpp\
				mainwindow.cpp \
				logindialog.cpp \
				settingsdialog.cpp \
				databasesettingspage.cpp \
				tabwidget.cpp \
				homepage.cpp \
				aboutdialog.cpp \
				encoding.cpp \
    database.cpp

HEADERS		+=	mainwindow.h \
				logindialog.h \
				settingsdialog.h \
				abstractsettingspage.h \
				databasesettingspage.h \
				tabwidget.h \
				homepage.h \
				aboutdialog.h \
				encoding.h \
    database.h

FORMS		+=	mainwindow.ui \
				logindialog.ui \
				settingsdialog.ui \
				databasesettingspage.ui

RESOURCES	+= ../resources/resources.qrc
