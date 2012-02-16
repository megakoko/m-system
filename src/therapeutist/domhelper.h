#pragma once

#include <QDomElement>

QDomElement addElement(QDomElement &parent, const QString &tagName,
					   const QString &text = QString::null);
