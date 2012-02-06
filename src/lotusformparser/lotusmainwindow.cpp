#include "lotusmainwindow.h"

#include <QFileInfo>
#include <QUrl>


LotusMainWindow::LotusMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	setAcceptDrops(true);
	init();
}


void LotusMainWindow::init()
{
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 2);

	connect(m_parse, SIGNAL(clicked()), SLOT(parse()));
}


void LotusMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasUrls())
		event->acceptProposedAction();
}


void LotusMainWindow::dropEvent(QDropEvent *event)
{
	foreach(const QUrl& url, event->mimeData()->urls())
	{
		const QFileInfo& info(url.toString().remove("file://"));

		if(info.isFile())
		{
			m_files << info.filePath();
			m_inputFiles->addItem(info.fileName());
		}
	}
}


void LotusMainWindow::parse()
{
	QList<UiElement> uiElements = FormParser::parseFiles(m_files);

	foreach(const UiElement element, uiElements)
		qDebug() << element.textId << element.label << element.enumValues;
}
