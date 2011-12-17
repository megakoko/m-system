QT			+=	core gui sql
TARGET		 =	loader
TEMPLATE	 =	app


include( ../m-system-config.pri )


SOURCES		+=	main.cpp\
				mainwindow.cpp \
				logindialog.cpp \
				passwords.cpp \
				settingsdialog.cpp \
				abstractsettingspage.cpp \
				databasesettingspage.cpp \
				tabwidget.cpp \
				homepage.cpp \
				aboutdialog.cpp \
				encoding.cpp

HEADERS		+=	mainwindow.h \
				logindialog.h \
				passwords.h \
				settingsdialog.h \
				abstractsettingspage.h \
				databasesettingspage.h \
				tabwidget.h \
				homepage.h \
				aboutdialog.h \
				encoding.h

FORMS		+=	mainwindow.ui \
				logindialog.ui \
				settingsdialog.ui \
				databasesettingspage.ui

RESOURCES	+= ../resources/resources.qrc