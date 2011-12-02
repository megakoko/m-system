TARGET	 = mkb10
TEMPLATE = lib
CONFIG	+= plugin
QT		+= sql

include( ../m-system-config.pri )


HEADERS +=	mkb10.h \
    mainmkb10widget.h

SOURCES +=	mkb10.cpp \
    mainmkb10widget.cpp

FORMS += \
    mainmkb10widget.ui
