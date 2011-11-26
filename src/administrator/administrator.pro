TARGET	 = administrator
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	administrator.cpp \
			adminwidget.cpp \
			../loader/passwords.cpp \		# FIXIT
			usereditwidget.cpp

HEADERS +=	administrator.h \
			adminwidget.h \
			../loader/passwords.h \		# FIXIT
			usereditwidget.h

FORMS	+= 	adminwidget.ui \
			usereditwidget.ui
