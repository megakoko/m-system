TARGET	= patients
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS += ../loader/pluginwidget.h \
	patients.h \
	mainpatientswidget.h

SOURCES += \
	patients.cpp \
	mainpatientswidget.cpp

FORMS += \
	mainpatientswidget.ui
