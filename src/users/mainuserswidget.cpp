#include "mainuserswidget.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

#include "usereditwidget.h"

#include "macros.h"


static const QString userListQuery =
		QString::fromUtf8("SELECT id, login AS Логин FROM MUser ORDER BY id");


MainUsersWidget::MainUsersWidget(QWidget *parent) :
	PluginWidget(parent)
{
	setupUi(this);
	init();
	initConnections();
	userSelectionChanged();
}


void MainUsersWidget::init()
{
	m_model = new QSqlQueryModel(this);
	updateUserList();

	m_view->setModel(m_model);
	m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_view->setColumnHidden(0, true);
}


void MainUsersWidget::initConnections()
{
	connect(m_view->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(userSelectionChanged()));

	connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(editUser()));

	connect(m_createUser, SIGNAL(clicked()), SLOT(createUser()));
	connect(m_editUser, SIGNAL(clicked()), SLOT(editUser()));
	connect(m_deleteUser, SIGNAL(clicked()), SLOT(deleteUser()));
}


void MainUsersWidget::updateUserList()
{
	m_model->setQuery(userListQuery);
	checkQuery(m_model->query());
}



bool MainUsersWidget::singleUserSelected() const
{
	return	m_view->selectionModel()->hasSelection() &&
			m_view->selectionModel()->selectedRows(0).count() == 1;
}


int MainUsersWidget::selectedUserId() const
{
	int id = -1;
	if (singleUserSelected())
		id = m_model->record(m_view->currentIndex().row()).value(0).toInt();

	return id;
}


QString MainUsersWidget::selectedUserName() const
{
	QString result;
	if (singleUserSelected())
		result = m_model->record(m_view->currentIndex().row()).value(1).toString();

	return result;
}


void MainUsersWidget::userSelectionChanged()
{
	const bool singleUser = singleUserSelected();

	m_editUser->setEnabled(singleUser);
	m_deleteUser->setEnabled(singleUser);
}


void MainUsersWidget::createUser()
{
	UserEditWidget* widget = new UserEditWidget(SaveablePluginWidget::InvalidId, this);
	connect(widget, SIGNAL(saved()), SLOT(updateUserList()));
	addNewWidget(widget, widget->fullUserName());
}


void MainUsersWidget::editUser()
{
	const int userid = selectedUserId();
	Q_ASSERT(userid >= 0);

	UserEditWidget* widget = new UserEditWidget(userid, this);
	connect(widget, SIGNAL(saved()), SLOT(updateUserList()));
	addNewWidget(widget, widget->fullUserName());
}


void MainUsersWidget::deleteUser()
{
	const int userid = selectedUserId();
	Q_ASSERT(userid >= 0);


	// Попытка удаления последнего пользователя с правами админа.
	if(UserEditWidget::userIsAdmin(userid) && UserEditWidget::numberOfAdminUsers() == 1)
	{
		static const QString& desc =
			QString::fromUtf8("Невозможно удалить данного пользователя, так как он "
							  "является последним пользователем с правами администратора.");

		QMessageBox::information(this, QString::fromUtf8("Удаление администратора"), desc);
	}
	else
	{
		const int rc = QMessageBox::question(this, "Удаление пользователя",
											 "Вы действительно хотите удалить пользователя?",
											 QMessageBox::Yes | QMessageBox::No,
											 QMessageBox::No);

		if(rc == QMessageBox::Yes)
		{
			QSqlQuery q;

			q.prepare("DELETE FROM UserPluginAccess WHERE userid = :userid");
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
}
