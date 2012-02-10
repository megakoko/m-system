TARGET	 = therapeutist
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql webkit xml

include( ../m-system-config.pri )


LIBS		=  -lcomponents -L$${ROOTPATH}/bin/
INCLUDEPATH	+=	$${ROOTPATH}/src/

# Передаем линковщику опцию -R$${ROOTPATH}/bin/
unix {
	QMAKE_CXXFLAGS += -Wl,-R$${ROOTPATH}/bin/
}


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
			examinationpreview.cpp

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
			examinationpreview.h

FORMS	+=	maintherapeutistwidget.ui \
			examinationeditwidget.ui \
			patientpickerdialog.ui \
			examinationpreview.ui
