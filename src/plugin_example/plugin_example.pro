TARGET    = pluginexample
TEMPLATE  = lib
CONFIG   += plugin
QT       += sql

include( ../m-system-config.pri )

SOURCES += pluginexample.cpp pluginexamplewidget.cpp 
HEADERS += pluginexample.h   pluginexamplewidget.h
