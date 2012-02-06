#pragma once

#include <QString>
#include <QStringList>
#include <QDebug>


struct UiElement
{
	QString textId;

	QString label;
	QStringList enumValues;

	bool isNull() const
	{	return label.isNull() && enumValues.isEmpty();	}
};


class QDomElement;

namespace FormParser
{
	QList<UiElement> parseFiles(const QStringList& fileNames);

	QList<UiElement> parseFile(const QByteArray &xml);
	UiElement parseTableRow(const QDomElement& tableRow);

	QString extractTextId(const QDomElement& tableRow);
	QString extractLabel(const QDomElement& tableCell);
	QStringList extractEnumValues(const QDomElement& tableCell);
}
