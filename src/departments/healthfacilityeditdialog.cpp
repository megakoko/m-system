#include "healthfacilityeditdialog.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "departments.h"
#include "macros.h"

HealthFacilityEditDialog::HealthFacilityEditDialog(QWidget *parent)
	: QDialog(parent)
{
    setupUi(this);
	init();
}


void HealthFacilityEditDialog::init()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(save()));
	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));


	m_name->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("HealthFacility", "name"));

	m_shortName->setMaxLength(Departments::interfaces->db->
							  fieldMaximumLength("HealthFacility", "shortname"));

	m_city->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("HealthFacility", "city"));

	m_street->setMaxLength(Departments::interfaces->db->
						   fieldMaximumLength("HealthFacility", "street"));

	m_house->setMaxLength(Departments::interfaces->db->
						  fieldMaximumLength("HealthFacility", "house"));

	m_inn->setMaxLength(Departments::interfaces->db->
						fieldMaximumLength("HealthFacility", "inn"));

	m_kpp->setMaxLength(Departments::interfaces->db->
						fieldMaximumLength("HealthFacility", "kpp"));

	m_okonh->setMaxLength(Departments::interfaces->db->
						  fieldMaximumLength("HealthFacility", "okonh"));

	m_okato->setMaxLength(Departments::interfaces->db->
						  fieldMaximumLength("HealthFacility", "okato"));

	m_okpo->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("HealthFacility", "okpo"));

	m_ogrn->setMaxLength(Departments::interfaces->db->
						 fieldMaximumLength("HealthFacility", "ogrn"));


	QSqlQuery q(" SELECT name, shortName, city, street, house, "
					" inn, kpp, okonh, okato, okpo, ogrn "
				" FROM HealthFacility ");
	checkQuery(q);

	const bool hasRecord = q.first();
	Q_ASSERT(hasRecord); Q_UNUSED(hasRecord);

	if(hasRecord)
	{
		const QSqlRecord& rec = q.record();

		m_name->setText(rec.value("name").toString());
		m_shortName->setText(rec.value("shortName").toString());
		m_city->setText(rec.value("city").toString());
		m_street->setText(rec.value("street").toString());
		m_house->setText(rec.value("house").toString());
		m_inn->setText(rec.value("inn").toString().trimmed());
		m_kpp->setText(rec.value("kpp").toString().trimmed());
		m_okonh->setText(rec.value("okonh").toString().trimmed());
		m_okato->setText(rec.value("okato").toString().trimmed());
		m_okpo->setText(rec.value("okpo").toString().trimmed());
		m_ogrn->setText(rec.value("ogrn").toString().trimmed());
	}
}


void HealthFacilityEditDialog::save()
{
	QSqlQuery q;
	q.prepare(" UPDATE HealthFacility SET "
			  " name = :name, "
			  " shortName = :shortName, "
			  " city = :city, "
			  " street = :street, "
			  " house = :house, "
			  " inn = :inn, "
			  " kpp = :kpp, "
			  " okonh = :okonh, "
			  " okato = :okato, "
			  " okpo = :okpo, "
			  " ogrn = :ogrn ");
	q.bindValue(":name", nullIfEmpty(m_name->text().simplified()));
	q.bindValue(":shortName", nullIfEmpty(m_shortName->text().simplified()));
	q.bindValue(":city", nullIfEmpty(m_city->text().simplified()));
	q.bindValue(":street", nullIfEmpty(m_street->text().simplified()));
	q.bindValue(":house", nullIfEmpty(m_house->text().simplified()));
	q.bindValue(":inn", nullIfEmpty(m_inn->text().simplified()));
	q.bindValue(":kpp", nullIfEmpty(m_kpp->text().simplified()));
	q.bindValue(":okonh", nullIfEmpty(m_okonh->text().simplified()));
	q.bindValue(":okato", nullIfEmpty(m_okato->text().simplified()));
	q.bindValue(":okpo", nullIfEmpty(m_okpo->text().simplified()));
	q.bindValue(":ogrn", nullIfEmpty(m_ogrn->text().simplified()));
	q.exec();

	checkQuery(q);
}


QString HealthFacilityEditDialog::shortInformation()
{
	QStringList result;

	QSqlQuery q("SELECT name, shortName, inn, kpp FROM HealthFacility");
	const bool hasRecord = q.first();

	checkQuery(q);

	Q_ASSERT(hasRecord); Q_UNUSED(hasRecord);

	if(hasRecord)
	{
		const QSqlRecord& rec = q.record();

		if(rec.value("shortName").isNull())
			result << rec.value("name").toString();
		else
			result << rec.value("name").toString() +
					  " (" + rec.value("shortName").toString() + ")";

		if(!rec.value("inn").isNull())
			result << QString::fromUtf8("ИНН: ") + rec.value("inn").toString().trimmed();


		if(!rec.value("kpp").isNull())
			result << QString::fromUtf8("КПП: ") + rec.value("kpp").toString().trimmed();
	}

	return result.join(", ");
}
