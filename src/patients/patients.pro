TARGET	 = patients
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

LIBS		=  -lcomponents -L$${ROOTPATH}/bin/
INCLUDEPATH	+=	$${ROOTPATH}/src/

# Передаем линковщику опцию -R$${ROOTPATH}/bin/
unix {
	QMAKE_CXXFLAGS += -Wl,-R$${ROOTPATH}/bin/
}


HEADERS +=	patients.h \
			mainpatientswidget.h \
			patienteditwidget.h \
			address.h \
			addressdialog.h \
			document.h \
			documenteditdialog.h

SOURCES +=	patients.cpp \
			mainpatientswidget.cpp \
			patienteditwidget.cpp \
			address.cpp \
			addressdialog.cpp \
			document.cpp \
			documenteditdialog.cpp

FORMS += 	mainpatientswidget.ui \
			patienteditwidget.ui \
			addressdialog.ui \
			documenteditdialog.ui
