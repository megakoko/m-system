#pragma once

#include "databaseinterface.h"


class Database : public DatabaseInterface
{
public:
	int fieldMaximumLength(const QString& table, const QString& field) const;
	bool fieldIsNullable(const QString &table, const QString &field) const;

};
