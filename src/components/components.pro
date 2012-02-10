# Библиотека с различными компонентами, используемыми в модулях системы.
TARGET	 = components
TEMPLATE = lib

include( ../m-system-config.pri )

HEADERS	+=	sortfilterproxymodel.h \
			decodedpatientlistquery.h

SOURCES +=	sortfilterproxymodel.cpp \
			decodedpatientlistquery.cpp
