TARGET	 = departments
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	departments.h \
			maindepartmentswidget.h \
			staffeditdialog.h \
			departmenteditwidget.h \
			healthfacilityeditdialog.h \
			positionseditdialog.h \
			staffpositioneditdialog.h \
			staffposition.h

SOURCES +=	departments.cpp \
			maindepartmentswidget.cpp \
			staffeditdialog.cpp \
			departmenteditwidget.cpp \
			healthfacilityeditdialog.cpp \
			positionseditdialog.cpp \
			staffpositioneditdialog.cpp \
			staffposition.cpp

FORMS	+= 	maindepartmentswidget.ui \
			staffeditdialog.ui \
			departmenteditwidget.ui \
			healthfacilityeditdialog.ui \
			positionseditdialog.ui \
			staffpositioneditdialog.ui
