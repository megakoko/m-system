TARGET	 = diagnosis_rules
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

LIBS	+=  -lcomponents -L$${ROOTPATH}/bin/

HEADERS +=	diagnosisrules.h \
			maindiagnosiswidget.h \
			ruleeditwidget.h \
			ruleitem.h \
			symptompickerdialog.h \
			ruleitemeditdialog.h

SOURCES	+=	diagnosisrules.cpp \
			maindiagnosiswidget.cpp \
			ruleeditwidget.cpp \
			ruleitem.cpp \
			symptompickerdialog.cpp \
			ruleitemeditdialog.cpp

FORMS	+=	maindiagnosiswidget.ui \
			ruleeditwidget.ui \
			symptompickerdialog.ui \
			ruleitemeditdialog.ui
