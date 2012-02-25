TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql xml

include( ../m-system-config.pri )


LIBS		=  -lcomponents -L$${ROOTPATH}/bin/


SOURCES +=	therapeutist.cpp \
			maintherapeutistwidget.cpp \
			examinationeditwidget.cpp \
			examwidget.cpp \
			examcontainer.cpp \
			examwidgetfactory.cpp \
			examcombobox.cpp \
			examlineedit.cpp \
			patientpickerdialog.cpp \
			examinputwidget.cpp \
			examtextedit.cpp \
			plaintextedit.cpp \
			examspinbox.cpp \
			examinationpreview.cpp \
			therapeutistpickerdialog.cpp \
			examinationlistreport.cpp \
			domhelper.cpp \
			daterangedialog.cpp \
			examdiagnosisedit.cpp

HEADERS	+=	therapeutist.h \
			maintherapeutistwidget.h \
			examinationeditwidget.h \
			examwidget.h \
			examcontainer.h \
			examwidgetfactory.h \
			examcombobox.h \
			examlineedit.h \
			patientpickerdialog.h \
			examinputwidget.h \
			examtextedit.h \
			plaintextedit.h \
			examspinbox.h \
			examinationpreview.h \
			therapeutistpickerdialog.h \
			examinationlistreport.h \
			domhelper.h \
			daterangedialog.h \
			examdiagnosisedit.h

FORMS	+=	maintherapeutistwidget.ui \
			examinationeditwidget.ui \
			patientpickerdialog.ui \
			examinationpreview.ui \
			therapeutistpickerdialog.ui \
			examinationlistreport.ui \
			daterangedialog.ui
