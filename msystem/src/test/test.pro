TARGET    = test
TEMPLATE  = lib
CONFIG   += plugin
QT       += sql

include( ../m-system-config.pri )

SOURCES += test.cpp maintestwidget.cpp 
HEADERS += test.h   maintestwidget.h
