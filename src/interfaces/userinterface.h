#pragma once


class QString;

class UserInterface
{
public:
	/// Деструктор.
	virtual ~UserInterface() {}


	static const int InvalidId = 0;


	/// Возвращает идентификатор пользователя, который в данный момент работает в системе.
	/// Возвращает UserInterface::InvalidId, если идентификатор не может быть получен.
	virtual int currentUserId() const = 0;


	/// Возвращает true, если текущий пользователь является администратором.
	virtual bool currentUserIsAdmin() const = 0;


	/// Возвращает true, если текущий пользователь имеет доступ к модулю с
	/// идентификатором \a textid.
	virtual bool currentUserHasAccessToPlugin(const QString& textid) const = 0;

};
