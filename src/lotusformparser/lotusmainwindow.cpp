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

}


void LotusMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasUrls())
		event->acceptProposedAction();
}


void LotusMainWindow::dropEvent(QDropEvent *event)
{
	QStringList files;
	foreach(const QUrl& url, event->mimeData()->urls())
	{
		const QFileInfo& info(url.toLocalFile());

		if(info.isFile())
			files << info.filePath();
	}

	m_outputSql->setPlainText(parse(files, files.first(),
									m_containerTextId->text(), m_startId->value()));
}


QString LotusMainWindow::parse(const QStringList& filenames,
							   const QString& containerLabel,
							   const QString& containerTextid, const int startId)
{
	const int increment = 10;


	QList<UiElement> ui = FormParser::parseFiles(filenames);

	QString text;

	const QString uiElementTemplate = "SELECT %1, '%2', '%3', NULL, '%4', '%5'";
	QStringList uiElements;

	int id = startId;
	uiElements += uiElementTemplate.arg(id).arg(containerTextid).arg("main").arg("container").arg(containerLabel);
	id += increment;
	foreach(const UiElement& element, ui)
	{
		if(!element.textId.isNull())
		{
			QString type = element.enumValues.isEmpty() ? "lineedit" : "combobox";

			uiElements += uiElementTemplate.arg(id).arg(element.textId).arg(containerTextid).arg(type).arg(element.label);
			id += increment;
		}
	}
	text += "INSERT INTO UiElement(id, textId, parentId, availableForSexId, typeId, label)\n"
			+ uiElements.join(" UNION\n") + ";\n\n";


	id = startId;
	QStringList uiElementEnums;
	const QString uiElementEnumTemplate = "SELECT %1, '%2', '%3'";
	foreach(const UiElement& element, ui)
	{
		if(!element.textId.isNull())
		{
			foreach(const QString& value, element.enumValues)
			{
				uiElementEnums += uiElementEnumTemplate.arg(id).arg(element.textId).arg(value);
				id += increment;
			}
		}
	}
	text += "INSERT INTO UiElementEnums(id, uiElementTextId, value)\n"
			+ uiElementEnums.join(" UNION\n") + ";\n";


	return text;
}
