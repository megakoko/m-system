#include "domhelper.h"

QDomElement addElement(QDomElement &parent, const QString &tagName, const QString &text)
{
	Q_ASSERT(!parent.isNull());
	QDomDocument doc = parent.ownerDocument();

	QDomElement element = doc.createElement(tagName);
	parent.appendChild(element);
	if(!text.isNull())
		element.appendChild(doc.createTextNode(text));

	return element;
}

