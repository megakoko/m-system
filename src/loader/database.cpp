#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QSettings>
#include <QRegExp>
#include <QDebug>

#include "macros.h"


DatabaseInterface::SqlDriver Database::m_currentDriver = DatabaseInterface::Unknown;
bool Database::m_currentDriverValueWasInitialized = false;


int Database::fieldMaximumLength(QString table, QString field) const
{
	table = table.toLower();
	field = field.toLower();


	int length = -1;

	QSqlQuery q;
	switch (currentSqlDriver())
	{
	case PSQL:
		{
			q.prepare(" SELECT character_maximum_length "
					  " FROM information_schema.columns "
					  " WHERE table_name = :table AND column_name = :field ");
			q.bindValue(":table", table);
			q.bindValue(":field", field);
			q.exec();
			checkQuery(q);

			if(q.first() && !q.value(0).isNull())
				length = q.value(0).toInt();
		}
		break;
	case SQLITE:
		{
			/*
				В поле sql таблицы sqlite_master хранятся определения таблиц
				(для type='table'), то есть SQL-команда CREATE TABLE, из которой затем
				с помощью регулярного выражения будет "выдернуто" определение колонки
				нужной таблицы.
			*/
			q.prepare(" SELECT sql FROM sqlite_master "
					  " WHERE type = 'table' AND name like :table ");
			q.bindValue(":table", table);
			q.exec();
			checkQuery(q);

			if(q.first())
			{
				/*
					Пробельный символ, затем имя поля, затем несколько пробельных символов,
					затем "varchar" или "char", затем пробел, затем открывающая скобка "(",
					затем несколько цифр, затем закрывающая скобка ")".
				*/
				QRegExp rx("\\s" + field + "\\s+(?:var)?char\\s?\\((\\d+)\\)",
						   Qt::CaseInsensitive);
				if(rx.indexIn(q.value(0).toString()) > -1 && rx.captureCount() > 0)
					length = rx.cap(rx.captureCount()).toInt();
			}
		}
		break;
	default:
		qFatal("Unhandled sql driver");
	}


	if(fieldIsCiphered(table, field))
	{
		/*
		408 -- это максимальный размер зашифрованной строки из ста (100) UTF-8 символов.
		Если поле хранит зашифрованную строку, то оно должно иметь такую длину.
		*/
		static const int lengthOf100LengthCipheredString = 408;
		Q_ASSERT(length == lengthOf100LengthCipheredString);
		Q_UNUSED(lengthOf100LengthCipheredString);

		length = 100;
	}


	return length;
}


bool Database::fieldIsNullable(const QString &table, const QString &field) const
{
	bool isNullable = false;

	QSqlQuery q;
	switch (currentSqlDriver())
	{
	case PSQL:
		{
			q.prepare(" SELECT CAST(is_nullable AS BOOL) "
					  " FROM information_schema.columns "
					  " WHERE table_name = :table AND column_name = :field");
			q.bindValue(":table", table);
			q.bindValue(":field", field);
			q.exec();
			checkQuery(q);

			if(q.first() && !q.value(0).isNull())
				isNullable = q.value(0).toBool();
		}
		break;
	case SQLITE:
		break;
	default:
		qFatal("Unhandled sql driver");
	}

	return isNullable;
}


QMap<QString, QStringList> Database::initializeCipheredTablesMap() const
{
	// key -- название таблицы;
	// value -- QStringList из названий зашифрованных полей в таблице.
	QMap<QString, QStringList> map;

	map.insert("patient", QStringList() << "familyname" << "name" << "patronymic");
	map.insert("document", QStringList() << "serialnumber" << "givenby");
	map.insert("address", QStringList() << "city" << "street" << "house" << "apartment");

	return map;
}


bool Database::fieldIsCiphered(const QString& table, const QString& field) const
{
	static const QMap<QString, QStringList> cipheredTables = initializeCipheredTablesMap();

	return	cipheredTables.contains(table) &&
			cipheredTables.value(table).contains(field);
}


QString Database::returningSentence(const QString &field) const
{
	QString result;
	if(currentSqlDriver() == DatabaseInterface::PSQL)
		result = " RETURNING " + field;
	return result;
}


QVariant Database::lastInsertedId(QSqlQuery *const query) const
{
	QVariant id;
	if(query != NULL && query->isActive())
	{
		switch(currentSqlDriver())
		{
		case DatabaseInterface::SQLITE:
			id = query->lastInsertId();
			break;
		case DatabaseInterface::PSQL:
			if (query->first())
				id = query->value(0);
			break;
		default:
			qFatal("Unknown sql driver");
		}
	}

	return id;
}


DatabaseInterface::SqlDriver Database::currentSqlDriver() const
{
	if(!m_currentDriverValueWasInitialized)
	{
		m_currentDriver = sqlDriver(QSettings().value("dbdriver").toString());
		m_currentDriverValueWasInitialized = true;
	}

	return m_currentDriver;
}


DatabaseInterface::SqlDriver Database::sqlDriver(const QString &driverName)
{
	SqlDriver driver;

	if (driverName == "QSQLITE")
		driver = SQLITE;
	else if (driverName == "QPSQL")
		driver = PSQL;
	else
		driver = Unknown;

	return driver;
}


void Database::setSqlDriver(const QString &driverName)
{
	m_currentDriver = sqlDriver(driverName);
}
