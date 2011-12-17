TARGET	 = administrator
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	administrator.cpp \
			mainadminwidget.cpp \
			../loader/passwords.cpp \		# FIXIT
			usereditwidget.cpp

HEADERS +=	administrator.h \
			mainadminwidget.h \
			../loader/passwords.h \		# FIXIT
			usereditwidget.h

FORMS	+= 	mainadminwidget.ui \
			usereditwidget.ui
