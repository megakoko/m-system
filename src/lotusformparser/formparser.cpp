#include "formparser.h"

#include <QDomDocument>
#include <QDomElement>

#include <QFile>
#include <QDebug>


const QRegExp trailingPunctuation("[.,: ]+$");


QList<UiElement> FormParser::parseFiles(const QStringList &fileNames)
{
	QFile file;
	QList<UiElement> result;

	foreach(const QString& fileName, fileNames)
	{
		file.setFileName(fileName);
		if(file.open(QIODevice::ReadOnly))
		{
			result.append(parseFile(file.readAll()));
			file.close();
		}
	}
	return result;
}


QList<UiElement> FormParser::parseFile(const QByteArray &xml)
{
	QList<UiElement> result;

	QString errorMsg;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if(!doc.setContent(xml, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "At line" << errorLine
				 << ", column" << errorColumn
				 << ":" << errorMsg;
	}
	else
	{
		QDomNodeList tableRows = doc.elementsByTagName("tablerow");

		for(int i = 0; i < tableRows.size(); ++i)
			if(tableRows.at(i).isElement())
				result += parseTableRow(tableRows.at(i).toElement());
	}

	return result;
}


UiElement FormParser::parseTableRow(const QDomElement &tableRow)
{
	UiElement ui;

	QDomNodeList tableCells = tableRow.elementsByTagName("tablecell");

	if(tableCells.size() < 2)
		qWarning() << "Less than two <tablecell> elements";
	else
	{
		ui.textId = extractTextId(tableRow);
		ui.label = extractLabel(tableCells.at(0).toElement());
		ui.enumValues = extractEnumValues(tableCells.at(1).toElement());
	}

	return ui;
}


QString FormParser::extractTextId(const QDomElement& tableRow)
{
	QString textId;

	const QDomNodeList& fields = tableRow.elementsByTagName("field");
	for(int i = 0; i < fields.size(); ++i)
	{
		const QDomElement& element = fields.at(i).toElement();
		if(element.hasAttribute("name"))
		{
			textId = element.attribute("name");
			break;
		}
	}

	return textId;
}


QString FormParser::extractLabel(const QDomElement& tableCell)
{
	QString label;


	// Сначала загляни в элемент <textlist>.
	const QDomNodeList& textLists = tableCell.elementsByTagName("textlist");
	if(textLists.size() > 0)
		label = textLists.at(0).toElement().text();
	else // Если элементов <textlist> нен нашли, поищи в элементе <par>.
	{
		const QDomNodeList& pars = tableCell.elementsByTagName("par");
		if(pars.size() > 0)
			label = pars.at(0).toElement().text();
	}

	return label.remove(trailingPunctuation);
}


QStringList FormParser::extractEnumValues(const QDomElement& tableCell)
{
	QStringList enumValues;

	const QDomNodeList& textLists = tableCell.elementsByTagName("textlist");
	if(textLists.size() > 0)
	{
		const QDomNodeList& texts = textLists.at(0).toElement().elementsByTagName("text");
		for(int i = 0; i < texts.size(); ++i)
			enumValues += texts.at(i).toElement().text().remove(trailingPunctuation);
	}

	return enumValues;
}
