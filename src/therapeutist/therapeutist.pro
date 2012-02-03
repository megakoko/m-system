TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


SOURCES +=	therapeutist.cpp \
			maintherapeutistwidget.cpp \
			examinationeditwidget.cpp \
			examwidget.cpp \
			examcontainer.cpp \
			examwidgetfactory.cpp \
			examcombobox.cpp \
			examlineedit.cpp

HEADERS +=	therapeutist.h \
			maintherapeutistwidget.h \
			examinationeditwidget.h \
			examwidget.h \
			examcontainer.h \
			examwidgetfactory.h \
			examcombobox.h \
			examlineedit.h

FORMS += maintherapeutistwidget.ui \
			examinationeditwidget.ui
