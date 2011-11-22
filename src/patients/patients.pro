TARGET	= patients
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	../loader/pluginwidget.h \
			patients.h \
			mainpatientswidget.h \
			patienteditwidget.h \
			address.h \
			addressdialog.h

SOURCES += \
			patients.cpp \
			mainpatientswidget.cpp \
			patienteditwidget.cpp \
			address.cpp \
			addressdialog.cpp

FORMS += \
			mainpatientswidget.ui \
			patienteditwidget.ui \
			addressdialog.ui
