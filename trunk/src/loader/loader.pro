

QT			+=	core gui sql
TARGET		=	loader
TEMPLATE	=	app


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
				aboutdialog.cpp

HEADERS		+=	mainwindow.h \
				logindialog.h \
				passwords.h \
				settingsdialog.h \
				abstractsettingspage.h \
				databasesettingspage.h \
				test.h \
				plugininterface.h \
				tabwidget.h \
				homepage.h \
				pluginwidget.h \
				macros.h \
				aboutdialog.h

FORMS		+=	mainwindow.ui \
				logindialog.ui \
				settingsdialog.ui \
				databasesettingspage.ui

RESOURCES += \
    ../resources/resources.qrc
