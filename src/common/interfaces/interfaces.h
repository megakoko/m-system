#pragma once

#include <QSharedPointer>
#include "encodinginterface.h"
#include "databaseinterface.h"

/// Интерфейсы для связи модуля с программой.
class Interfaces
{
public:
	/// Конструктор.
	Interfaces(EncodingInterface* enc, DatabaseInterface* db)
		: enc(enc)
		, db(db)
	{				}

	/// Деструктор. Здесь должны удаляться все подынтерфейсы.
	~Interfaces()
	{
		delete enc;
		delete db;
	}

	/// Шифрование.
	const EncodingInterface * const enc;
	/// База данных.
	const DatabaseInterface * const db;
};

typedef QSharedPointer<Interfaces> InterfacesPtr;

