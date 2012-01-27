TARGET	 = users
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	users.cpp \
			mainuserswidget.cpp \
			usereditwidget.cpp

HEADERS +=	users.h \
			mainuserswidget.h \
			usereditwidget.h

FORMS	+= 	mainuserswidget.ui \
			usereditwidget.ui
