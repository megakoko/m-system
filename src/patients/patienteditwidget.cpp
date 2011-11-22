#include "patienteditwidget.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../loader/macros.h"

#include "addressdialog.h"


PatientEditWidget::PatientEditWidget(const int patientId, QWidget *parent)
	: PluginWidget(parent)
	, m_patientId(patientId)
	, m_mailingAddress(Address(true))
	, m_actualAddress(Address(false))
{
	setupUi(this);

	init();
	initConnections();
}


void PatientEditWidget::init()
{
	QSqlQuery q;
	q.prepare(" SELECT familyName, name, patronymic "
			  " FROM Patient "
			  " WHERE id = :patientId");
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);

	const bool idIsValid = q.first();
	Q_ASSERT(idIsValid);

	m_familyName->setText(q.value(0).toString());
	m_name->setText(q.value(1).toString());
	m_patronymic->setText(q.value(2).toString());

	q.prepare(" SELECT id, isMailingAddress, city, street, house, apartment "
			  " FROM Address "
			  " WHERE patientId = :patientId "
			  " ORDER BY isMailingAddress DESC "); // TRUE, then FALSE
	q.bindValue(":patientId", m_patientId);
	q.exec();


	if(q.first())
	{
		m_mailingAddress = Address(q.record());
		if(q.next())
			m_actualAddress = Address(q.record());
	}

	m_mailingAddressLabel->setText(m_mailingAddress.toString());
	m_actualAddressLabel->setText(m_actualAddress.toString());

}


void PatientEditWidget::initConnections()
{
	connect(m_mailingAddressIsActual,
			SIGNAL(toggled(bool)),
			SLOT(toggleMailingAddressIsActual(bool)));

	connect(m_editMailingAddress, SIGNAL(clicked()), SLOT(launchMailingAddressEditing()));
	connect(m_editActualAddress, SIGNAL(clicked()), SLOT(launchActualAddressEditing()));

	connect(m_save, SIGNAL(clicked()), SLOT(save()));
}


void PatientEditWidget::toggleMailingAddressIsActual(bool checked)
{
	m_actualAddressLabel->setEnabled(!checked);
	m_editActualAddress->setEnabled(!checked);
}


void PatientEditWidget::launchMailingAddressEditing()
{
	AddressDialog d(m_mailingAddress, this);
	if(d.exec() == QDialog::Accepted)
	{
		m_mailingAddress = d.address();
		m_mailingAddressLabel->setText(m_mailingAddress.toString());
	}
}


void PatientEditWidget::launchActualAddressEditing()
{
	AddressDialog d(m_actualAddress, this);
	if(d.exec() == QDialog::Accepted)
	{
		m_actualAddress = d.address();
		m_actualAddressLabel->setText(m_actualAddress.toString());
	}
}


void PatientEditWidget::save()
{
	QSqlQuery q;
	q.prepare(" UPDATE Patient SET "
			  " familyName = :familyName, "
			  " name = :name, "
			  " patronymic = :patronymic "
			  " WHERE id = :patientId ");
	q.bindValue(":familyName", m_familyName->text());
	q.bindValue(":name", m_name->text());
	q.bindValue(":patronymic", m_patronymic->text());
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);


	m_mailingAddress.save(m_patientId);
	m_actualAddress.save(m_patientId);
}
