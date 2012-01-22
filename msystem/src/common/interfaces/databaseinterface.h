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
	virtual int fieldMaximumLength(const QString& table, const QString& field) const = 0;
	
	/// Возаращает true, если поле \a field таблицы \a table может принимать NULL.
	virtual bool fieldIsNullable(const QString& table, const QString& field) const = 0;

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
