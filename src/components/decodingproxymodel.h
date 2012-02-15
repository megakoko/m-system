#pragma once

#include <QProxyModel>
#include <QSet>

#include "interfaces/interfaces.h"


class DecodingProxyModel : public QProxyModel
{
public:
	explicit DecodingProxyModel(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const;

	void addEncodedStringColumn(const int column);
	void addEncodedDateColumn(const int column);

	void setInterfacesPtr(const InterfacesPtr& interfaces);

private:
	bool columnIsEncoded(const int column) const;
	QVariant decodeData(const QModelIndex& index) const;

	QSet<int> m_columnsToDecode;
	QSet<int> m_dateColumnsToDecode;

	InterfacesPtr m_interfaces;

};
