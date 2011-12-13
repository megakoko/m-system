TARGET	 = departments
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	departments.h \
			maindepartmentswidget.h \
			staffeditwidget.h \
			departmeneditwidget.h \
			healthfacilityeditdialog.h

SOURCES +=	departments.cpp \
			maindepartmentswidget.cpp \
			staffeditwidget.cpp \
			departmeneditwidget.cpp \
			healthfacilityeditdialog.cpp

FORMS	+= 	maindepartmentswidget.ui \
			staffeditwidget.ui \
			departmeneditwidget.ui \
			healthfacilityeditdialog.ui
