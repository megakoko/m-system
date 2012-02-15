#pragma once

#include "interfaces/databaseinterface.h"
#include <QString>
#include <QMap>


class Database : public DatabaseInterface
{
public:
	int fieldMaximumLength(QString table, QString field) const;

	SqlDriver currentSqlDriver() const;
	static void setSqlDriver(const QString& driverName);

	QString returningSentence(const QString &field) const;
	QVariant lastInsertedId(QSqlQuery *const query) const;


private:
	/// Возвращает true, если поле хранит зашифрованные значения.
	bool fieldIsCiphered(const QString& table, const QString& field) const;

	QMap<QString, QStringList> initializeCipheredTablesMap() const;

	/// Возвращает элемент перечисления SqlDriver, соответствующий имени драйвера
	/// \a driverName.
	static SqlDriver sqlDriver(const QString& driverName);

	/// Драйвер, который используется программой в настоящий момент.
	static SqlDriver m_currentDriver;

	/// Определяет, было ли инициализировано значение текущего драйвера.
	static bool m_currentDriverValueWasInitialized;

};
