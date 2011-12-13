#include "healthfacilityeditdialog.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

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


	m_inn->setMaxLength(12);
	m_kpp->setMaxLength(9);
	m_okonh->setMaxLength(5);
	m_okato->setMaxLength(11);
	m_okpo->setMaxLength(10);
	m_ogrn->setMaxLength(13);


	QSqlQuery q(" SELECT name, shortName, inn, kpp, okonh, okato, okpo, ogrn "
				" FROM HealthFacility ");
	checkQuery(q);

	const bool hasRecord = q.first();
	Q_ASSERT(hasRecord); Q_UNUSED(hasRecord);

	if(hasRecord)
	{
		const QSqlRecord& rec = q.record();

		m_name->setText(rec.value("name").toString());
		m_shortName->setText(rec.value("shortName").toString());
		m_inn->setText(rec.value("inn").toString().trimmed());
		m_kpp->setText(rec.value("kpp").toString().trimmed());
		m_okonh->setText(rec.value("okonh").toString().trimmed());
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
			  " inn = :inn, "
			  " kpp = :kpp, "
			  " okonh = :okonh, "
			  " okato = :okato, "
			  " okpo = :okpo, "
			  " ogrn = :ogrn ");
	q.bindValue(":name", nullIfEmpty(m_name->text().simplified()));
	q.bindValue(":shortName", nullIfEmpty(m_shortName->text().simplified()));
	q.bindValue(":inn", nullIfEmpty(m_inn->text().simplified()));
	q.bindValue(":kpp", nullIfEmpty(m_kpp->text().simplified()));
	q.bindValue(":okonh", nullIfEmpty(m_okonh->text().simplified()));
	q.bindValue(":okato", nullIfEmpty(m_okato->text().simplified()));
	q.bindValue(":okpo", nullIfEmpty(m_okpo->text().simplified()));
	q.bindValue(":ogrn", nullIfEmpty(m_ogrn->text().simplified()));
	q.exec();

	checkQuery(q);
}


QString HealthFacilityEditDialog::toString()
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
			result << QString::fromUtf8("ИНН: ") + rec.value("inn").toString();


		if(!rec.value("kpp").isNull())
			result << QString::fromUtf8("КПП: ") + rec.value("kpp").toString();
	}

	return result.join(", ");
}
