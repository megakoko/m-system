#include "usereditwidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QMap>
#include <QDateTime>
#include <QDebug>

#include "administrator.h"
#include "macros.h"


UserEditWidget::UserEditWidget(const int userId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_userId(userId)
{
	setupUi(this);

	initConnections();
	init();

	loginEdited();
}


void UserEditWidget::init()
{
	m_login->setMaxLength(Administrator::interfaces->
						  db->fieldMaximumLength("MUser", "login"));

	const int pwLength = Administrator::interfaces->
						 db->fieldMaximumLength("MUser", "password");
	m_password->setMaxLength(pwLength);
	m_password2->setMaxLength(pwLength);


	QSqlQuery query;
	query.exec("SELECT textid, name FROM Plugin");
	checkQuery(query);
	while(query.next())
	{
		const QString& textid = query.value(0).toString();

		QCheckBox* checkbox = new QCheckBox(query.value(1).toString(), this);
		m_checkboxToTextid[checkbox] = textid;
		m_textidToCheckbox[textid] = checkbox;

		verticalLayout->insertWidget(verticalLayout->count()-1, checkbox, 0, Qt::AlignTop);
	}


	if(m_userId != InvalidId)
	{
		query.prepare(" SELECT login, is_admin "
					  " FROM MUser "
					  " WHERE id = :userid ");
		query.bindValue(":userid", m_userId);
		query.exec();
		checkQuery(query);

		query.first();
		m_login->setText(query.value(0).toString());

		const bool isAdmin = query.value(1).toBool();

		m_isAdmin->setChecked(isAdmin);


		if(!isAdmin)
		{
			query.prepare(" SELECT p.textid "
						  " FROM UserPluginAccess upa "
						  " LEFT JOIN Plugin p ON upa.pluginid = p.id "
						  " WHERE upa.userid = :userid ");
			query.bindValue(":userid", m_userId);
			query.exec();
			checkQuery(query);


			// Снимаем галки со всех QCheckBox.
			QMap<QString, QCheckBox*>::iterator i = m_textidToCheckbox.begin();
			while(i != m_textidToCheckbox.end())
			{
				i.value()->setChecked(false);
				++i;
			}

			while(query.next())
			{
				const QString& textid = query.value(0).toString();
				if(m_textidToCheckbox.contains(textid))
					m_textidToCheckbox[textid]->setChecked(true);
			}
		}
	}
}


void UserEditWidget::initConnections()
{
	connect(m_isAdmin, SIGNAL(toggled(bool)), SLOT(isAdminToggled(bool)));
	connect(m_save, SIGNAL(clicked()), SIGNAL(closeMe()));

	connect(m_login, SIGNAL(editingFinished()), SLOT(loginEdited()));
}


bool UserEditWidget::loginIsUnique() const
{
	QSqlQuery q;

	if(m_userId == InvalidId)
		q.prepare("SELECT COUNT(*) FROM MUser WHERE login = :login");
	else
	{
		q.prepare("SELECT COUNT(*) FROM MUser WHERE login = :login AND id <> :id");
		q.bindValue(":id", m_userId);
	}
	q.bindValue(":login", m_login->text());

	q.exec();
	checkQuery(q);

	q.first();
	return q.value(0).toInt();
}


bool UserEditWidget::canSave(QString& errorDescription) const
{
	if (userIsAdmin(m_userId) &&
		numberOfAdminUsers() == 1 &&
		m_isAdmin->isChecked() == false)
	{
		errorDescription = QString::fromUtf8("Попытка удалить права администратора "
						"у последнего администратора.");
		return false;
	}
	else if (m_login->text().simplified().isEmpty())
	{
		errorDescription  = QString::fromUtf8("Имя пользователя не заполнено.");
		return false;
	}
	else if (loginIsUnique())
	{
		errorDescription = "Имя пользователя должно быть уникально.";
		return false;
	}
	else if (m_password->text() != m_password2->text())
	{
		errorDescription = QString::fromUtf8("Подтверждение пароля не совпадает с паролем.");
		return false;
	}


	return true;
}


void UserEditWidget::save()
{
	QSqlQuery query;
	if(m_password->text().simplified().isEmpty())
	{
		if(m_userId == InvalidId)
		{
			query.prepare(" INSERT INTO MUser "
						  " ( login,  is_admin) VALUES "
						  " (:login, :is_admin) RETURNING id ");
		}
		else
		{
			query.prepare(" UPDATE MUser SET "
						  " login = :login, "
						  " is_admin = :is_admin "
						  " WHERE id = :userid ");
			query.bindValue(":userid", m_userId);
		}
	}
	else
	{
		if(m_userId == InvalidId)
		{
			query.prepare(" INSERT INTO MUser "
						  " ( login,  password,  salt,  is_admin) VALUES "
						  " (:login, :password, :salt, :is_admin) "
						  " RETURNING id");
		}
		else
		{
			query.prepare(" UPDATE MUser SET "
						  " login = :login, "
						  " password = :password, "
						  " salt = :salt, "
						  " is_admin = :is_admin "
						  " WHERE id = :userid ");
			query.bindValue(":userid", m_userId);
		}

		const QString& salt = Administrator::interfaces->enc->salt(15);
		const QString& pw	= Administrator::interfaces->
							  enc->password(m_password->text(), salt);

		query.bindValue(":password", pw);
		query.bindValue(":salt", salt);
	}
	query.bindValue(":login", m_login->text());
	query.bindValue(":is_admin", m_isAdmin->isChecked());
	query.exec();
	checkQuery(query);


	if(m_userId == InvalidId)
	{
		query.first();
		m_userId = query.value(0).toInt();
	}


	const QSet<QString>& textidsBefore = databaseTextids();
	const QSet<QString>& textidsAfter  = checkedTextids();

	const QSet<QString>& remove = QSet<QString>(textidsBefore).subtract(textidsAfter);
	const QSet<QString>& add = QSet<QString>(textidsAfter).subtract(textidsBefore);


	query.prepare(" DELETE FROM UserPluginAccess "
				  " WHERE userid = :userid "
				  " AND pluginid = (SELECT id FROM Plugin WHERE textid = :textid) ");
	foreach(const QString& textid, remove)
	{
		query.bindValue(":userid", m_userId);
		query.bindValue(":textid", textid);
		query.exec();
		checkQuery(query);
	}

	query.prepare(" INSERT INTO UserPluginAccess (userid, pluginid) "
				  " VALUES (:userid, (SELECT id FROM Plugin WHERE textid = :textid) )");
	foreach(const QString& textid, add)
	{
		query.bindValue(":userid", m_userId);
		query.bindValue(":textid", textid);
		query.exec();
		checkQuery(query);
	}

	emit saved();
}


QSet<QString> UserEditWidget::databaseTextids() const
{
	QSet<QString> result;

	QSqlQuery query;
	query.prepare(" SELECT p.textid "
				  " FROM UserPluginAccess upa "
				  " LEFT JOIN Plugin p ON upa.pluginid = p.id "
				  " WHERE upa.userid = :userid ");
	query.bindValue(":userid", m_userId);
	query.exec();
	checkQuery(query);

	while(query.next())
		result << query.value(0).toString();

	return result;
}


QSet<QString> UserEditWidget::checkedTextids() const
{
	QSet<QString> result;

	QMap<QCheckBox*, QString>::const_iterator i = m_checkboxToTextid.constBegin();
	while(i != m_checkboxToTextid.constEnd())
	{
		if(i.key()->isChecked())
			result << i.value();
		++i;
	}

	return result;
}


void UserEditWidget::isAdminToggled(bool checked)
{
	m_scrollAreaWidgetContents->setHidden(checked);
	m_pluginAccessLabel->setHidden(checked);
}


bool UserEditWidget::userIsAdmin(int userid)
{
	QSqlQuery q;
	q.prepare("SELECT is_admin FROM MUser WHERE id = :userid");
	q.bindValue(":userid", userid);
	q.exec();
	checkQuery(q);

	bool result = false;
	if(q.first())
		result = q.value(0).toBool();

	return result;
}


int UserEditWidget::numberOfAdminUsers()
{
	QSqlQuery q("SELECT COUNT(*) FROM MUser WHERE is_admin = TRUE");
	checkQuery(q);

	int result = 0;
	if(q.first())
		result = q.value(0).toInt();

	return result;
}


QString UserEditWidget::fullUserName() const
{
	return QString::fromUtf8("Пользователь %1").arg(m_login->text());
}


void UserEditWidget::loginEdited()
{
	emit setTabLabel(fullUserName());
}
