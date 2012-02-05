TARGET	 = dummy_db
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

RESOURCES+= resources/resources.qrc

SOURCES	 +=	\
			dummydatabase.cpp \
			maindummydatabasewidget.cpp

HEADERS  += dummydatabase.h \
			maindummydatabasewidget.h

FORMS    += maindummydatabasewidget.ui
