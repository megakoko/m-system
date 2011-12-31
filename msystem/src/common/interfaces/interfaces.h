#pragma once

#include <QSharedPointer>
#include "encodinginterface.h"


class Interfaces
{
public:
	Interfaces(EncodingInterface* enc)
		: enc(enc)
	{				}

	~Interfaces()
	{
		delete enc;
	}

	const EncodingInterface * const enc;
};

typedef QSharedPointer<Interfaces> InterfacesPtr;
