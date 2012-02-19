TARGET	 = diagnosis_rules
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	diagnosisrules.h \
			maindiagnosiswidget.h

SOURCES	+=	diagnosisrules.cpp \
			maindiagnosiswidget.cpp

FORMS	+=	maindiagnosiswidget.ui
