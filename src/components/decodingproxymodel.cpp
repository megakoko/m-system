#include "decodingproxymodel.h"

#include <QDebug>


DecodingProxyModel::DecodingProxyModel(QObject *parent)
	: QProxyModel(parent)
{
}


QVariant DecodingProxyModel::data(const QModelIndex &index, int role) const
{
	Q_ASSERT(!m_interfaces.isNull());

	QVariant result = QProxyModel::data(index, role);

	if(role == Qt::DisplayRole && m_columnsToDecode.contains(index.column()))
	{
		Q_ASSERT(result.type() == QVariant::String);
		result = m_interfaces->enc->decode(result.toString());
	}

	return result;
}


void DecodingProxyModel::addColumnToDecode(const int column)
{
	m_columnsToDecode << column;
}


void DecodingProxyModel::setInterfacesPtr(const InterfacesPtr &interfaces)
{
	m_interfaces = interfaces;
}
