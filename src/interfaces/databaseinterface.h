#pragma once

class QString;
class QVariant;
class QSqlQuery;


/// Интерфейс, содержащий некоторые методы для работы с БД.
class DatabaseInterface
{
public:
	/// Возаращет вместимость CHAR-поля \a field таблицы \a table либо -1, если поле
	/// не имеет тип CHAR/VARCHAR, не имеет максимальной длины.
	virtual int fieldMaximumLength(QString table, QString field) const = 0;


	/// Доступные SQL драйвера.
	enum SqlDriver {
		SQLITE,
		PSQL,
		Unknown
	};

	/// Возвращает имя используемого в настоящий момент драйвера.
	virtual SqlDriver currentSqlDriver() const = 0;

	virtual QString returningSentence(const QString& field) const = 0;

	virtual QVariant lastInsertedId(QSqlQuery* const query) const = 0;

};
