#pragma once

#include <QSortFilterProxyModel>


/**
	Переопределенная QSortFilterProxyModel, нечувствительная к букве "Ё".
*/
class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
	SortFilterProxyModel(QObject* parent = NULL);

protected:
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
	/// Возвращает true, если \a str содержит букву "Ё".
	static bool containsYo(const QString& str);

	/// Заменяет все вхождения буквы "Ё" на букву "Е". Аналогично для нижнего регистра.
	static QString replaceYo(const QString& str);

};
