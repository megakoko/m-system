

CONFIG( plugin ) {
	PLUGINDIR = "plugins"
}

ROOTPATH = ../../

DESTDIR		=	$${ROOTPATH}/bin/$${PLUGINDIR}
OBJECTS_DIR =	$${ROOTPATH}/bin/objects
MOC_DIR		=	$${ROOTPATH}/bin/moc
UI_DIR		=	$${ROOTPATH}/bin/ui
RCC_DIR		=	$${ROOTPATH}/bin/rcc

INCLUDEPATH	+=	$${ROOTPATH}/src/common \
				$${ROOTPATH}/src/common/interfaces

HEADERS		+=	$${ROOTPATH}/src/common/macros.h \
				$${ROOTPATH}/src/common/pluginwidget.h \
				$${ROOTPATH}/src/common/plugininterface.h \
				$${ROOTPATH}/src/common/saveablepluginwidget.h

# Интерфейсы:
HEADERS		+=	$${ROOTPATH}/src/common/interfaces/interfaces.h \
				$${ROOTPATH}/src/common/interfaces/databaseinterface.h \
				$${ROOTPATH}/src/common/interfaces/encodinginterface.h
