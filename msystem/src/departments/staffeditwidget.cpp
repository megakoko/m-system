#include "staffeditwidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"


StaffEditWidget::StaffEditWidget(const int staffId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_staffId(staffId)
{
    setupUi(this);

	init();
	initConnections();
}


void StaffEditWidget::init()
{
	Q_ASSERT(m_staffId > 0);


	if(m_staffId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT familyName, name, patronymic, birthday, specialization "
				  " FROM Staff "
				  " WHERE id = :id ");
		q.addBindValue(m_staffId);
		q.exec();
		checkQuery(q);

		const bool idIsValid = q.first();
		Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

		m_familyName->setText(q.value(0).toString());
		m_name->setText(q.value(1).toString());
		m_patronymic->setText(q.value(2).toString());
		m_birthDay->setDate(q.value(3).toDate());
		m_specialization->setText(q.value(4).toString());
	}
}


void StaffEditWidget::initConnections()
{
	connect(m_familyName, SIGNAL(editingFinished()), SLOT(nameChanged()));
	connect(m_name, SIGNAL(editingFinished()), SLOT(nameChanged()));
	connect(m_patronymic, SIGNAL(editingFinished()), SLOT(nameChanged()));

	connect(m_save, SIGNAL(clicked()), SIGNAL(closeMe()));
}


QString StaffEditWidget::staffName() const
{
	QString result = "Работник";

	if(!m_familyName->text().simplified().isEmpty())
	{
		result += " " + m_familyName->text();

		if(!m_name->text().trimmed().isEmpty() &&
		   !m_patronymic->text().trimmed().isEmpty())
		{
			result += " " + m_name->text().trimmed().left(1) + "." +
					  m_patronymic->text().trimmed().left(1) + ".";

		}
	}

	return result;
}


bool StaffEditWidget::canSave(QString &errorDescription) const
{
	if (m_familyName->text().simplified().isEmpty() ||
		m_name->text().simplified().isEmpty() ||
		m_patronymic->text().simplified().isEmpty())
	{
		errorDescription = QString::fromUtf8("Имя сотрудника не заполнено");
		return false;
	}
	else if(m_specialization->text().simplified().isEmpty())
	{
		errorDescription = QString::fromUtf8("Специализация сотрудника не заполнена");
		return false;
	}

	return true;
}


void StaffEditWidget::save()
{
	QSqlQuery q;

	if(m_staffId == InvalidId)
	{
		q.prepare(" INSERT INTO Staff "
				  " ( familyName,  name,  patronymic,  birthday,  specialization) VALUES"
				  " (:familyName, :name, :patronymic, :birthday, :specialization) ");
	}
	else
	{
		q.prepare(" UPDATE Staff SET "
				  " familyName = :familyName, "
				  " name = :name, "
				  " patronymic = :patronymic, "
				  " birthday = :birthday, "
				  " specialization = :specialization "
				  " WHERE id = :id ");
		q.bindValue(":id", m_staffId);
	}
	q.bindValue(":familyName", nullIfEmpty(m_familyName->text().simplified()));
	q.bindValue(":name", nullIfEmpty(m_name->text().simplified()));
	q.bindValue(":patronymic", nullIfEmpty(m_patronymic->text().simplified()));
	q.bindValue(":birthday", m_birthDay->date());
	q.bindValue(":specialization", m_specialization->text().simplified());
	q.exec();
	checkQuery(q);

	emit saved();
}


void StaffEditWidget::nameChanged()
{
	emit setTabLabel(staffName());
}
