#pragma once

#include <QProxyModel>
#include <QSet>

#include "interfaces.h"


// TODO: Вынести создание такой модели в отдельный интерфейс.
class DecodedPatientListQuery : public QProxyModel
{
public:
	explicit DecodedPatientListQuery(QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const;

	void addColumnToDecode(const int column);

	void setInterfacesPtr(const InterfacesPtr& interfaces);
private:
	QSet<int> m_columnsToDecode;

	InterfacesPtr m_interfaces;

};
