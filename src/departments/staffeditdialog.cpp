#include "staffeditdialog.h"

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "macros.h"
#include "departments.h"


StaffEditDialog::StaffEditDialog(const int staffId, QWidget *parent)
	: QDialog(parent)
	, m_staffId(staffId)
{
    setupUi(this);

	init();
	initConnections();
}


void StaffEditDialog::init()
{
	m_familyName->setMaxLength(Departments::interfaces->db->
							   fieldMaximumLength("Staff", "familyName"));

	m_name->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("Staff", "name"));

	m_patronymic->setMaxLength(Departments::interfaces->db->
							   fieldMaximumLength("Staff", "patronymic"));

	m_specialization->setMaxLength(Departments::interfaces->db->
								   fieldMaximumLength("Staff", "specialization"));


	if(m_staffId != 0) // TODO
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

	checkFields();
}


void StaffEditDialog::initConnections()
{
	connect(m_familyName, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_name, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_patronymic, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_specialization, SIGNAL(textChanged(QString)), SLOT(checkFields()));

	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(save()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));
}


void StaffEditDialog::checkFields()
{
	const bool fieldsAreValid = !m_familyName->text().simplified().isEmpty() &&
								!m_name->text().simplified().isEmpty() &&
								!m_patronymic->text().simplified().isEmpty() &&
								!m_specialization->text().simplified().isEmpty();

	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(fieldsAreValid);
}


void StaffEditDialog::save()
{
	QSqlQuery q;

	if(m_staffId == 0)	// TODO
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
}
