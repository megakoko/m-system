TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	therapeutist.cpp \
			maintherapeutistwidget.cpp

HEADERS +=	therapeutist.h \
			maintherapeutistwidget.h

FORMS += maintherapeutistwidget.ui
