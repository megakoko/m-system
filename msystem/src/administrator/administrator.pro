TARGET	 = administrator
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	administrator.cpp \
			mainadminwidget.cpp \
			usereditwidget.cpp

HEADERS +=	administrator.h \
			mainadminwidget.h \
			usereditwidget.h

FORMS	+= 	mainadminwidget.ui \
			usereditwidget.ui
