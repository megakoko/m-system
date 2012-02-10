#include "columnjoiningproxymodel.h"

#include <QStringList>



ColumnJoiningProxyModel::ColumnJoiningProxyModel(QObject *parent)
	: QProxyModel(parent)
	, m_firstJoinedColumn(-1)
	, m_numberOfJoinedColumns(-1)
	, m_columnSeparator(" ")
{

}


void ColumnJoiningProxyModel::setJoinedColumns(const int firstColumn, const int number,
											   const QString &header)
{
	m_firstJoinedColumn = firstColumn;
	m_numberOfJoinedColumns = number;
	m_joinedColumnsHeader = header;
}


void ColumnJoiningProxyModel::setColumnSeparator(const QString &separator)
{
	m_columnSeparator = separator;
}


int ColumnJoiningProxyModel::columnCount(const QModelIndex &parent) const
{
	return QProxyModel::columnCount(parent) - (m_numberOfJoinedColumns - 1);
}


QVariant ColumnJoiningProxyModel::data(const QModelIndex &index, int role) const
{
	QVariant result;

	if(role != Qt::DisplayRole || m_firstJoinedColumn == -1)
		result = QProxyModel::data(index, role);
	else
	{
		const int row = index.row();
		const int column = index.column();

		if(column < m_firstJoinedColumn)
			result = QProxyModel::data(QProxyModel::index(row, index.column()));
		else if(column == m_firstJoinedColumn)
		{
			QStringList columns;
			const int lastJoinedColumn = m_firstJoinedColumn + m_numberOfJoinedColumns - 1;

			for(int i = m_firstJoinedColumn; i <= lastJoinedColumn; ++i)
				columns << QProxyModel::data(QProxyModel::index(row, i)).toString();

			result = columns.join(m_columnSeparator);
		}
		else /* if(index.column() > m_firstJoinedColumn) */
			result = QProxyModel::data(QProxyModel::index(row, column +
														  (m_numberOfJoinedColumns - 1)));
	}

	return result;
}


QVariant ColumnJoiningProxyModel::headerData(int section, Qt::Orientation orientation,
											 int role) const
{
	QVariant result;

	if(role != Qt::DisplayRole || orientation != Qt::Horizontal || m_firstJoinedColumn == -1)
		result = QProxyModel::headerData(section, orientation, role);
	else
	{
		if(section < m_firstJoinedColumn)
			result = QProxyModel::headerData(section, orientation, role);
		else if(section == m_firstJoinedColumn)
			result = m_joinedColumnsHeader;
		else /* if(section > m_firstJoinedColumn) */
			result = QProxyModel::headerData(section + (m_numberOfJoinedColumns-1),
											 orientation, role);
	}



	return result;
}
