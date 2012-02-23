#pragma once

#include <QSharedPointer>
#include "encodinginterface.h"
#include "databaseinterface.h"
#include "userinterface.h"
#include "demointerface.h"

/// Интерфейсы для связи модуля с программой.
class Interfaces
{
public:
	/// Конструктор.
	Interfaces(EncodingInterface* enc, DatabaseInterface* db, UserInterface* usr, DemoInterface* demo)
		: enc(enc)
		, db(db)
		, usr(usr)
		, demo(demo)
	{				}

	/// Деструктор. Здесь должны удаляться все подынтерфейсы.
	~Interfaces()
	{
		delete enc;
		delete db;
		delete usr;
		delete demo;
	}

	/// Шифрование.
	const EncodingInterface * const enc;
	/// База данных.
	const DatabaseInterface * const db;
	/// Пользователь.
	const UserInterface * const usr;
	/// Демо-версия.
	const DemoInterface * const demo;

};

typedef QSharedPointer<Interfaces> InterfacesPtr;

