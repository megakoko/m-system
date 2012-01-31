TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	therapeutist.cpp \
			maintherapeutistwidget.cpp \
			examinationeditwidget.cpp

HEADERS +=	therapeutist.h \
			maintherapeutistwidget.h \
			examinationeditwidget.h

FORMS += maintherapeutistwidget.ui \
			examinationeditwidget.ui
