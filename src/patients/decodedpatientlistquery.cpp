#include "decodedpatientlistquery.h"

#include <QDebug>
#include "patients.h"


DecodedPatientListQuery::DecodedPatientListQuery(QObject *parent)
	: QProxyModel(parent)
{
}


QVariant DecodedPatientListQuery::data(const QModelIndex &index, int role) const
{
	QVariant result = QProxyModel::data(index, role);

	if(role == Qt::DisplayRole && m_columnsToDecode.contains(index.column()))
	{
		Q_ASSERT(result.type() == QVariant::String);
		result = Patients::interfaces->enc->decode(result.toString());
	}

	return result;
}


void DecodedPatientListQuery::addColumnToDecode(const int column)
{
	m_columnsToDecode << column;
}
