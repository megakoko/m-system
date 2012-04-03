#include "addressdialog.h"
#include "macros.h"
#include "patients.h"

#include <QDebug>
#include <QPushButton>
#include <QCompleter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>


AddressDialog::AddressDialog(const Address& address, QWidget *parent)
	: QDialog(parent)
	, m_address(address)
{
	setupUi(this);

	m_region->setMaxLength(Patients::interfaces->db->
						   fieldMaximumLength("Address", "region"));

	m_city->setMaxLength(Patients::interfaces->db->
						 fieldMaximumLength("Address", "city"));

	m_street->setMaxLength(Patients::interfaces->db->
						   fieldMaximumLength("Address", "street"));

	m_house->setMaxLength(Patients::interfaces->db->
						  fieldMaximumLength("Address", "house"));

	m_apartment->setMaxLength(Patients::interfaces->db->
							  fieldMaximumLength("Address", "apartment"));


	m_region->setText(m_address.region);
	m_city->setText(m_address.city);
	m_street->setText(m_address.street);
	m_house->setText(m_address.house);
	m_apartment->setText(m_address.apartment);
	checkFields();


	resize(width(), 0);


	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));

	connect(m_city, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_street, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_house, SIGNAL(textChanged(QString)), SLOT(checkFields()));


	if(kladrIsAvailable())
	{
		m_regionModel = new QSqlQueryModel;
		m_region->setCompleter(new QCompleter(m_regionModel));
		m_region->completer()->setCaseSensitivity(Qt::CaseInsensitive);
		updateRegionCompletition();


		m_cityModel = new QSqlQueryModel;
		m_city->setCompleter(new QCompleter(m_cityModel));
		m_city->completer()->setCaseSensitivity(Qt::CaseInsensitive);
		updateCityCompletition();


		m_streetModel = new QSqlQueryModel;
		m_street->setCompleter(new QCompleter(m_streetModel));
		m_street->completer()->setCaseSensitivity(Qt::CaseInsensitive);
		updateStreetCompletition();


		updateHouseCompletition();;


		connect(m_region, SIGNAL(editingFinished()), SLOT(updateCityCompletition()));
		connect(m_city, SIGNAL(editingFinished()), SLOT(updateStreetCompletition()));
		connect(m_street, SIGNAL(editingFinished()), SLOT(updateHouseCompletition()));
	}
}


Address AddressDialog::address()
{
	m_address.region	= nullIfEmpty(m_region->text().simplified());
	m_address.city		= nullIfEmpty(m_city->text().simplified());
	m_address.street	= nullIfEmpty(m_street->text().simplified());
	m_address.house		= nullIfEmpty(m_house->text().simplified());
	m_address.apartment	= nullIfEmpty(m_apartment->text().simplified());

	return m_address;
}


void AddressDialog::checkFields()
{
	const bool fieldsAreValid =
			!m_region->text().simplified().isEmpty() &&
			!m_city->text().simplified().isEmpty() &&
			!m_street->text().simplified().isEmpty() &&
			!m_house->text().simplified().isEmpty();


	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(fieldsAreValid);
}


bool tableExists(const QString& name)
{
	QSqlQuery q(QString("SELECT 1 FROM %1 LIMIT 1").arg(name));

	return q.isActive();
}


bool AddressDialog::kladrIsAvailable() const
{
	return	tableExists("kladr") && tableExists("street") && tableExists("doma");
}


QString AddressDialog::getCode(const QString& name,
							   const int level,
							   const QString& tableName,
							   const QString& codeBeginning)
{
	Q_ASSERT(1 <= level && level <= 6);

	QString code;

	if(!name.isEmpty())
	{
		QSqlQuery q;
		q.prepare(QString(" SELECT code "
						  " FROM %1 t"
						  " LEFT JOIN socrbase ON t.socr = socrbase.scname "
						  " WHERE (name = ? OR name || ', ' || socr = ?) "
						  " AND level = :level "
						  " AND code LIKE '%2%' ").arg(tableName).arg(codeBeginning));
		q.addBindValue(name);
		q.addBindValue(name);
		q.addBindValue(level);
		q.exec();
		checkQuery(q);

		if(q.first())
			code = q.value(0).toString();
	}

	return code;
}


QString AddressDialog::getRegionCode() const
{
	const QSet<QString> udmurtia = QSet<QString>() << "удмуртия"
												   << "удмуртская республика"
												   << "республика удмуртия";

	// Небольшой хак для случая, когда введено не то, что КЛАДР подсказывает.
	if(udmurtia.contains(m_region->text().toLower()))
		return "1800000000000";

	return getCode(m_region->text(), 1, "kladr", QString::null);
}


QString AddressDialog::getCityCode(const QString& codeBeginning) const
{
	QString code = getCode(m_city->text(), 3, "kladr", codeBeginning);
	if(code.isNull())
		code = getCode(m_city->text(), 4, "kladr", codeBeginning);
	return code;
}


QString AddressDialog::getStreetCode(const QString& codeBeginning) const
{
	return getCode(m_street->text(), 5, "street", codeBeginning);
}


bool AddressDialog::cityBelongsToRegion(const QString& cityCode, const QString& regionCode)
{
	return !cityCode.isNull() && cityCode.startsWith(regionCode.left(2));
}


bool AddressDialog::streetBelongsToCity(const QString& streetCode, const QString& cityCode)
{
	return !streetCode.isNull() && streetCode.startsWith(cityCode.left(cityCode.size()-2));
}


void AddressDialog::updateRegionCompletition()
{
	m_regionModel->setQuery(" SELECT name || ', ' || socr "
							" FROM kladr "
							" WHERE code LIKE '__00000000000' "
							" ORDER BY name ");
}


void AddressDialog::updateCityCompletition()
{
	const QString& regionCode = getRegionCode();

	QString query;
	if(!regionCode.isNull())
	{
		query = QString(" SELECT name || ', ' || socr "
						" FROM kladr "
						" WHERE code LIKE '%1_________00' "
						" ORDER BY name ").arg(regionCode.left(2));
	}

	qDebug() << __FUNCTION__;
	m_cityModel->setQuery(query);
}


void AddressDialog::updateStreetCompletition()
{
	const QString& regionCode = getRegionCode();
	const QString& cityCode = getCityCode(regionCode.left(2));


	QString query;

	// Убеждаемся, что населенный пункт выбран из того же региона.
	if(cityBelongsToRegion(cityCode, regionCode))
	{
		query = QString(" SELECT name || ', ' || socr "
						" FROM street "
						" WHERE code LIKE '%1%' "
						" ORDER BY name ").arg(cityCode.left(cityCode.size()-2));
		// Из кода населенного пункта убираем 2 последних цифры -- признак актуальности.
	}

	m_streetModel->setQuery(query);
}


void AddressDialog::updateHouseCompletition()
{
	const QString& regionCode = getRegionCode();
	const QString& cityCode = getCityCode(regionCode.left(2));
	const QString& streetCode = getStreetCode(cityCode.left(cityCode.size()-2));


	QString query;


	if(streetBelongsToCity(streetCode, cityCode) &&
	   cityBelongsToRegion(cityCode, regionCode))
	{
		query = QString(" SELECT name "
						" FROM doma "
						" WHERE code LIKE '%1%' "
						" ORDER BY name ").arg(streetCode.left(streetCode.size()-2));
		// Из кода улицы убираем 2 последних цифры -- признак актуальности.
	}


	// КЛАДР может хранить дома в виде интервалов типа "Ч(2-30)" (четные дома с 2 по 30).
	// Поэтому придется вручную колбасить QStringList с номерами.
	QStringList houses;

	QSqlQuery q(query);
	while(q.next())
	{
		// (Н|Ч)\((\d+)-(\d+)\)
		QRegExp rx("(Н|Ч)\\((\\d+)-(\\d+)\\)");

		if(rx.indexIn(q.value(0).toString()) == -1)	// Здесь с регекспом делать нечего.
			houses << q.value(0).toString();
		else // Ога... попался.
		{
			const QStringList& captured = rx.capturedTexts();

			bool ok;

			// Первый номер.
			const int first = captured.at(2).toInt(&ok);
			if(!ok) continue;

			// Последний номер.
			const int last = captured.at(3).toInt(&ok);
			if(!ok) continue;


			for(int i = first; i <= last; i += 2)
				houses << QString::number(i);
		}
	}
	houses.sort();

	m_house->setCompleter(NULL);
	m_house->setCompleter(new QCompleter(houses));
}


/*
select s.level, code, name || ', ' || socr
from kladr LEFT JOIN socrbase s ON kladr.socr = s.scname
where code like '18_________00' AND s.level IN ('3','4')
order by code;
*/
