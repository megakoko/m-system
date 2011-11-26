#include "adminwidget.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

#include "usereditwidget.h"

#include "../loader/passwords.h"
#include "macros.h"


static const QString userListQuery =
		QString::fromUtf8("SELECT id, login AS Логин FROM MUser ORDER BY id");


AdminWidget::AdminWidget(QWidget *parent) :
	PluginWidget(parent)
{
	setupUi(this);
	init();
	initConnections();
	userSelectionChanged();
}


void AdminWidget::init()
{
	m_model = new QSqlQueryModel(this);
	m_model->setQuery(userListQuery);

	m_view->setModel(m_model);
	m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_view->setColumnHidden(0, true);
}


void AdminWidget::initConnections()
{
	connect(m_view->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(userSelectionChanged()));

	connect(m_createUser, SIGNAL(clicked()), SLOT(createUser()));
	connect(m_editUser, SIGNAL(clicked()), SLOT(editUser()));
	connect(m_deleteUser, SIGNAL(clicked()), SLOT(deleteUser()));
}


void AdminWidget::updateUserList()
{
	m_model->setQuery(userListQuery);
}



bool AdminWidget::singleUserSelected() const
{
	return	m_view->selectionModel()->hasSelection() &&
			m_view->selectionModel()->selectedRows(0).count() == 1;
}


int AdminWidget::selectedUserId() const
{
	int id = -1;
	if (singleUserSelected())
		id = m_model->record(m_view->currentIndex().row()).value(0).toInt();

	return id;
}


QString AdminWidget::selectedUserName() const
{
	QString result;
	if (singleUserSelected())
		result = m_model->record(m_view->currentIndex().row()).value(1).toString();

	return result;
}


void AdminWidget::userSelectionChanged()
{
	const bool singleUser = singleUserSelected();

	m_editUser->setEnabled(singleUser);
	m_deleteUser->setEnabled(singleUser);
}


void AdminWidget::createUser()
{
	QSqlQuery q;
	q.prepare(" INSERT INTO MUser (login, is_admin) "
			  " VALUES (:login, FALSE) ");
	q.bindValue(":login", generateLogin());
	q.exec();
	checkQuery(q);

	updateUserList();
}


void AdminWidget::editUser()
{
	const int userid = selectedUserId();
	Q_ASSERT(userid >= 0);

	addNewWidget(new UserEditWidget(userid, this),
				 QString::fromUtf8("Пользователь %1").arg(selectedUserName()));
}


void AdminWidget::deleteUser()
{
	const int userid = selectedUserId();
	Q_ASSERT(userid >= 0);


	// An attempt to delete last admin user. Not good!
	if(UserEditWidget::userIsAdmin(userid) && UserEditWidget::numberOfAdminUsers() == 1)
	{
		static const QString& desc =
			QString::fromUtf8("Невозможно удалить данного пользователя, так как он "
							  "является последним пользователем с правами администратора.");

		QMessageBox::information(this, QString::fromUtf8("Удаление администратора"), desc);
	}
	else
	{
		QSqlQuery q;

		q.prepare("DELETE FROM UserPluginAccess WHERE id = :userid");
		q.addBindValue(userid);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM MUser WHERE id = :userid");
		q.addBindValue(userid);
		q.exec();
		checkQuery(q);

		updateUserList();
	}
}


QString AdminWidget::generateLogin() const
{
	const QString& basename = QString::fromUtf8("Пользователь");


	/*
		Regexp explanation 'basename \\((\\d+)\\)$':
		basename => ( => \d+ => ) => $.
	*/
	QSqlQuery q;
	q.prepare(QString(" SELECT SUBSTRING(login, E'%1 \\\\((\\\\d+)\\\\)$') AS Number "
					  " FROM MUser ORDER BY Number").arg(basename));
	q.exec();
	checkQuery(q);


	QList<int> numbers;
	while(q.next())
	{
		if(q.isNull(0))	// Although null values will appear last, we can use 'break' here.
			continue;
		numbers << q.value(0).toInt();
	}

	int possibleNumber = 0;
	while(++possibleNumber > 0)
		if(!numbers.contains(possibleNumber))
			break;

	Q_ASSERT(possibleNumber > 0);
	return QString("%1 (%2)").arg(basename).arg(possibleNumber);
}
