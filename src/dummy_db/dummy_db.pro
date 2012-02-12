TARGET	 = dummy_db
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

RESOURCES+= resources/resources.qrc

SOURCES	 +=	\
			dummydatabase.cpp \
			maindummydatabasewidget.cpp \
			dummydata.cpp \
			dummypatients.cpp \
			dummydepartments.cpp \
			dummyexaminations.cpp

HEADERS  += dummydatabase.h \
			maindummydatabasewidget.h \
			dummydata.h \
			dummypatients.h \
			dummydepartments.h \
			dummyexaminations.h

FORMS    += maindummydatabasewidget.ui
