#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject* parent)
	: QSortFilterProxyModel(parent)
{

}


bool SortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	bool result = false;

	const QVariant& dataLeft = sourceModel()->data(left);
	const QVariant& dataRight = sourceModel()->data(right);

	if(dataLeft.type() == QVariant::String && dataRight.type() == QVariant::String)
	{
		const QString& leftString = replaceYo(dataLeft.toString());
		const QString& rightString = replaceYo(dataRight.toString());

		// QString::localeAwareCompare возвращает число меньше нуля,
		// если leftString меньше rightString.
		result = (QString::localeAwareCompare(leftString, rightString) < 0);
	}
	else
		result = QSortFilterProxyModel::lessThan(left, right);

	return result;
}


bool SortFilterProxyModel::containsYo(const QString& str)
{
	return str.contains(QChar(0x0401), Qt::CaseInsensitive);
}


QString SortFilterProxyModel::replaceYo(const QString& str)
{
	if(containsYo(str))
	{
		return QString(str).replace(QChar(0x0401), QChar(0x0415)). // ё -> е
							replace(QChar(0x0451), QChar(0x0435)); // Ё -> Е
	}
	else
		return str;
}
