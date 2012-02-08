TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

INCLUDEPATH += $${ROOTPATH}/src


SOURCES +=	therapeutist.cpp \
			maintherapeutistwidget.cpp \
			examinationeditwidget.cpp \
			$${ROOTPATH}/src/patients/decodedpatientlistquery.cpp \
			$${ROOTPATH}/src/patients/sortfilterproxymodel.cpp \
			examwidget.cpp \
			examcontainer.cpp \
			examwidgetfactory.cpp \
			examcombobox.cpp \
			examlineedit.cpp \
			patientpickerdialog.cpp \
			examinputwidget.cpp \
			examtextedit.cpp \
			plaintextedit.cpp

HEADERS	+=	therapeutist.h \
			maintherapeutistwidget.h \
			examinationeditwidget.h \
			$${ROOTPATH}/src/patients/decodedpatientlistquery.h \
			$${ROOTPATH}/src/patients/sortfilterproxymodel.h \
			examwidget.h \
			examcontainer.h \
			examwidgetfactory.h \
			examcombobox.h \
			examlineedit.h \
			patientpickerdialog.h \
			examinputwidget.h \
			examtextedit.h \
			plaintextedit.h

FORMS	+=	maintherapeutistwidget.ui \
			examinationeditwidget.ui \
			patientpickerdialog.ui
