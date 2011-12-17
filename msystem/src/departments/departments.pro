TARGET	 = departments
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	departments.h \
			maindepartmentswidget.h \
			staffeditwidget.h \
			departmenteditwidget.h \
			healthfacilityeditdialog.h \
			positionseditdialog.h \
			staffpositioneditdialog.h \
			staffposition.h

SOURCES +=	departments.cpp \
			maindepartmentswidget.cpp \
			staffeditwidget.cpp \
			departmenteditwidget.cpp \
			healthfacilityeditdialog.cpp \
			positionseditdialog.cpp \
			staffpositioneditdialog.cpp \
			staffposition.cpp

FORMS	+= 	maindepartmentswidget.ui \
			staffeditwidget.ui \
			departmenteditwidget.ui \
			healthfacilityeditdialog.ui \
			positionseditdialog.ui \
			staffpositioneditdialog.ui
