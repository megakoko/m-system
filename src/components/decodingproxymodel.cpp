#include "decodingproxymodel.h"

#include <QDate>
#include <QDebug>


DecodingProxyModel::DecodingProxyModel(QObject *parent)
	: QProxyModel(parent)
{
}


QVariant DecodingProxyModel::data(const QModelIndex &index, int role) const
{
	Q_ASSERT(!m_interfaces.isNull());

	QVariant result;

	if(role == Qt::DisplayRole && columnIsEncoded(index.column()))
		result = decodeData(index);
	else
		result = QProxyModel::data(index, role);

	return result;
}


bool DecodingProxyModel::columnIsEncoded(const int column) const
{
	return m_columnsToDecode.contains(column) || m_dateColumnsToDecode.contains(column);
}


QVariant DecodingProxyModel::decodeData(const QModelIndex &index) const
{
	QVariant result;

	if(m_columnsToDecode.contains(index.column()))
		result = m_interfaces->enc->decodeStr(QProxyModel::data(index).toString());
	else if(m_dateColumnsToDecode.contains(index.column()))
		result = m_interfaces->enc->decodeDate(QProxyModel::data(index).toString()).toString("dd.MM.yyyy");
	else
		Q_ASSERT(false);

	return result;
}


void DecodingProxyModel::addEncodedStringColumn(const int column)
{
	m_columnsToDecode << column;
}


void DecodingProxyModel::addEncodedDateColumn(const int column)
{
	m_dateColumnsToDecode << column;
}


void DecodingProxyModel::setInterfacesPtr(const InterfacesPtr &interfaces)
{
	m_interfaces = interfaces;
}
