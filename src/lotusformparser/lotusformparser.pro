QT		+= core gui xml
TARGET	 = lotusformparser
TEMPLATE = app

include(../m-system-config.pri)


SOURCES	+=	lotusmain.cpp\
			lotusmainwindow.cpp \
			formparser.cpp

HEADERS	+=	lotusmainwindow.h \
			formparser.h

FORMS	+=	lotusmainwindow.ui
