

CONFIG( plugin ) {
	PLUGINDIR = "plugins"
}

ROOTPATH	 = ../..

DESTDIR		 =	$${ROOTPATH}/bin/$${PLUGINDIR}
OBJECTS_DIR  =	$${ROOTPATH}/bin/objects/$${TARGET}
MOC_DIR		 =	$${ROOTPATH}/bin/moc/$${TARGET}
UI_DIR		 =	$${ROOTPATH}/bin/ui/$${TARGET}
RCC_DIR		 =	$${ROOTPATH}/bin/rcc/$${TARGET}


INCLUDEPATH +=	$${ROOTPATH}/src


HEADERS		+=	$${ROOTPATH}/src/interfaces/interfaces.h \
				$${ROOTPATH}/src/interfaces/databaseinterface.h \
				$${ROOTPATH}/src/interfaces/encodinginterface.h \
				$${ROOTPATH}/src/interfaces/userinterface.h


# Для проектов для плагинов и программы-загрузчика добавляем заголовки:
contains(TEMPLATE, app) | CONFIG(plugin) {
	INCLUDEPATH	+=	$${ROOTPATH}/src/common

	HEADERS		+=	$${ROOTPATH}/src/common/macros.h \
					$${ROOTPATH}/src/common/pluginwidget.h \
					$${ROOTPATH}/src/common/plugininterface.h \
					$${ROOTPATH}/src/common/saveablepluginwidget.h \
					$${ROOTPATH}/src/common/saveableobject.h
}
