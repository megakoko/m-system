#include "decodedpatientlistquery.h"

#include <QDebug>


DecodedPatientListQuery::DecodedPatientListQuery(QObject *parent)
	: QProxyModel(parent)
{
}


QVariant DecodedPatientListQuery::data(const QModelIndex &index, int role) const
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


void DecodedPatientListQuery::addColumnToDecode(const int column)
{
	m_columnsToDecode << column;
}


void DecodedPatientListQuery::setInterfacesPtr(const InterfacesPtr &interfaces)
{
	m_interfaces = interfaces;
}
