TARGET	 = departments
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	departments.h \
			maindepartmentswidget.h \
    staffeditwidget.h \
    departmeneditwidget.h

SOURCES +=	departments.cpp \
			maindepartmentswidget.cpp \
    staffeditwidget.cpp \
    departmeneditwidget.cpp

FORMS	+= 	maindepartmentswidget.ui \
    staffeditwidget.ui \
    departmeneditwidget.ui
