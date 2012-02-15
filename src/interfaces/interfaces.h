#pragma once

#include <QSharedPointer>
#include "encodinginterface.h"
#include "databaseinterface.h"
#include "userinterface.h"

/// Интерфейсы для связи модуля с программой.
class Interfaces
{
public:
	/// Конструктор.
	Interfaces(EncodingInterface* enc, DatabaseInterface* db, UserInterface* usr)
		: enc(enc)
		, db(db)
		, usr(usr)
	{				}

	/// Деструктор. Здесь должны удаляться все подынтерфейсы.
	~Interfaces()
	{
		delete enc;
		delete db;
		delete usr;
	}

	/// Шифрование.
	const EncodingInterface * const enc;
	/// База данных.
	const DatabaseInterface * const db;
	/// Пользователь.
	const UserInterface * const usr;

};

typedef QSharedPointer<Interfaces> InterfacesPtr;

