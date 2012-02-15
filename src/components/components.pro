# Библиотека с различными компонентами, используемыми в модулях системы.
TARGET	 = components
TEMPLATE = lib

include( ../m-system-config.pri )

HEADERS	+=	yosortfilterproxymodel.h \
			decodingproxymodel.h \
			columnjoiningproxymodel.h \
			searchwidget.h

SOURCES +=	yosortfilterproxymodel.cpp \
			decodingproxymodel.cpp \
			columnjoiningproxymodel.cpp \
			searchwidget.cpp
