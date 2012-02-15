#include "user.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"


int User::m_currentUserId = UserInterface::InvalidId;


int User::currentUserId() const
{
	return m_currentUserId;
}


void User::setCurrentUserId(const int id)
{
	m_currentUserId = id;
}


bool User::currentUserIsAdmin() const
{
	bool isAdmin = false;

	if(m_currentUserId != InvalidId)
	{
		QSqlQuery q;
		q.prepare("SELECT is_admin FROM MUser WHERE id = ?");
		q.addBindValue(m_currentUserId);
		q.exec();
		checkQuery(q);

		if(q.first())
			isAdmin = q.value(0).toBool();
	}

	return isAdmin;
}


bool User::currentUserHasAccessToPlugin(const QString &textid) const
{
	bool hasAccess = false;

	if(m_currentUserId != InvalidId)
	{
		if(currentUserIsAdmin())
			hasAccess = true;
		else
		{
			QSqlQuery q;
			q.prepare(" SELECT u.id FROM MUser u "
					  " LEFT JOIN UserPluginAccess upl ON u.id = upl.userid "
					  " LEFT JOIN Plugin p ON p.id = upl.pluginid "
					  " WHERE p.textid = :textid AND u.id = :userid ");
			q.bindValue(":textid", textid);
			q.bindValue(":userid", m_currentUserId);
			q.exec();
			checkQuery(q);

			hasAccess = q.first();
		}
	}

	return hasAccess;
}
