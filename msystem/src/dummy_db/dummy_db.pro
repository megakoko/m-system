TARGET	 = dummy_db
TEMPLATE = app
QT		+= sql

include( ../m-system-config.pri )


LIBS		=  -lcryptopp -L$${ROOTPATH}/bin/
INCLUDEPATH	+=	$${ROOTPATH}/src/

# Передаем линковщику опцию -R$${ROOTPATH}/bin/
unix {
	QMAKE_CXXFLAGS += -Wl,-R$${ROOTPATH}/bin/
}



RESOURCES+= resources/resources.qrc

SOURCES	 +=	main.cpp \
			mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
