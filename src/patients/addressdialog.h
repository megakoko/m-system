#pragma once

#include "ui_addressdialog.h"
#include "address.h"


class QSqlQueryModel;


class AddressDialog : public QDialog, private Ui::AddressDialog
{
	Q_OBJECT

public:
	explicit AddressDialog(const Address& address, QWidget *parent = 0);
	Address address();

private:
	bool kladrIsAvailable() const;

	static QString getCode(const QString& name, const int level, const QString& tableName, const QString &codeStart);

	QString getRegionCode() const;
	QString getCityCode(const QString& codeBeginning) const;
	QString getStreetCode(const QString& codeBeginning) const;

	static bool cityBelongsToRegion(const QString& cityCode, const QString& regionCode);
	static bool streetBelongsToCity(const QString& streetCode, const QString& cityCode);

	Address m_address;

	QSqlQueryModel* m_regionModel;
	QSqlQueryModel* m_cityModel;
	QSqlQueryModel* m_streetModel;
	QSqlQueryModel* m_houseModel;

private slots:
	void checkFields();

	void updateRegionCompletition();
	void updateCityCompletition();
	void updateStreetCompletition();
	void updateHouseCompletition();

};
