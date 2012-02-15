#pragma once

#include "interfaces/userinterface.h"

class User : public UserInterface
{
public:
	int currentUserId() const;
	static void setCurrentUserId(const int id);

	bool currentUserIsAdmin() const;

	bool currentUserHasAccessToPlugin(const QString &textid) const;

private:
	/// Идентификатор текущего пользователя.
	static int m_currentUserId;

};

