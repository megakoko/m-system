TARGET	 = diagnosis_rules
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	diagnosisrules.h \
			maindiagnosiswidget.h \
			ruleeditwidget.h \
			ruleitem.h

SOURCES	+=	diagnosisrules.cpp \
			maindiagnosiswidget.cpp \
			ruleeditwidget.cpp \
			ruleitem.cpp

FORMS	+=	maindiagnosiswidget.ui \
			ruleeditwidget.ui
