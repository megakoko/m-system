

CONFIG( plugin ) {
	PLUGINDIR = "plugins"
}

BASEPATH = ../../

DESTDIR		=	$${BASEPATH}/bin/$${PLUGINDIR}
OBJECTS_DIR =	$${BASEPATH}/bin/objects
MOC_DIR		=	$${BASEPATH}/bin/moc
UI_DIR		=	$${BASEPATH}/bin/ui
RCC_DIR		=	$${BASEPATH}/bin/rcc

HEADERS		+= ../loader/macros.h