TARGET	 = diagnosis_rules
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )

HEADERS +=	diagnosisrules.h

SOURCES	+=	diagnosisrules.cpp
