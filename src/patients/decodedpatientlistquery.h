#pragma once

#include <QProxyModel>
#include <QSet>

class DecodedPatientListQuery : public QProxyModel
{
public:
	explicit DecodedPatientListQuery(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const;

	void addColumnToDecode(const int column);

private:
	QSet<int> m_columnsToDecode;

};
