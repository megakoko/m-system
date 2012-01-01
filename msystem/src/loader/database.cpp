#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QDebug>

#include "macros.h"


int Database::fieldMaximumLength(const QString &table, const QString &field) const
{
	QSqlQuery q;
	q.prepare(" SELECT character_maximum_length "
			  " FROM information_schema.columns "
			  " WHERE table_name = :table AND column_name = :field ");
	q.bindValue(":table", table.toLower());
	q.bindValue(":field", field.toLower());
	q.exec();
	checkQuery(q);

	int result = -1;
	if(q.first() && !q.value(0).isNull())
		result = q.value(0).toInt();

	return result;
}


bool Database::fieldIsNullable(const QString &table, const QString &field) const
{
	QSqlQuery q;
	q.prepare(" SELECT CAST(is_nullable AS BOOL) "
			  " FROM information_schema.columns "
			  " WHERE table_name = :table AND column_name = :field");
	q.bindValue(":table", table);
	q.bindValue(":field", field);
	q.exec();
	checkQuery(q);

	bool result = false;
	if(q.first() && !q.value(0).isNull())
		result = q.value(0).toBool();

	return result;
}
