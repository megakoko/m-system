#pragma once

#include <QProxyModel>


/**
	Прокси-модель, которая объединяет заданные колонки исходной модели.
*/
class ColumnJoiningProxyModel : public QProxyModel
{
	Q_OBJECT
public:
	explicit ColumnJoiningProxyModel(QObject *parent = 0);

	///	Метод, который устанавливает первую из объединяющихся колонок \a firstColumn,
	///	количество объединяющихся колонок \a number, заголовок получаемой колонки
	///	\a header.
	void setJoinedColumns(const int firstColumn, const int number, const QString& header);

	/// Задает разделитель колонок \a separator.
	void setColumnSeparator(const QString& separator);


	/// Переопределенные методы.
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
	/// Первая колонка, которая учавствует в процессе объединения.
	int m_firstJoinedColumn;
	/// Количество колонок, учавствующих в процессе объединения.
	int m_numberOfJoinedColumns;
	/// Заголовок объединенной колонки.
	QString m_joinedColumnsHeader;

	/// Разделитель колонок.
	QString m_columnSeparator;
	
};
