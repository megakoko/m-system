# Библиотека с различными компонентами, используемыми в модулях системы.
TARGET	 = components
TEMPLATE = lib

include( ../m-system-config.pri )

HEADERS	+=	yosortfilterproxymodel.h \
			decodingproxymodel.h

SOURCES +=	yosortfilterproxymodel.cpp \
			decodingproxymodel.cpp
