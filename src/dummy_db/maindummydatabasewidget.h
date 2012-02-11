#pragma once


#include "pluginwidget.h"
#include "ui_maindummydatabasewidget.h"

class QFile;


struct Name
{
	QString surname;
	QString firstname;
	QString patronymic;
};


class MainDummyDatabaseWidget : public PluginWidget, private Ui::MainDummyDatabaseWidget
{
	Q_OBJECT

public:
	explicit MainDummyDatabaseWidget(QWidget *parent = 0);


private:
	void init();
	void initConnections();

	void loadFiles();
	void loadFile(const QString& fileName, QStringList& stringList);
	bool tryToOpen(QFile& f) const;
	void readFile(QFile& f, QStringList& stringList);

	// Возвращает зашифрованную с помощью CryptoPP строку.
	QString encode(const QString& plaintext) const;
	// Возвращает зашифрованную с помощью CryptoPP дату.
	QString encode(const QDate& plainDate) const;


	// Вставка в различные таблицы.

	// Возвращает ID пациента.
	int createPatientRecord(const QDate& birthday) const;
	void createDocumentRecord(const int patientId,
							  const QString& documentTextid,
							  const QDate& patientBirthday) const;
	void createAddressRecord(const int patientId, const QString& addressType) const;


	// Random-функции.
	void initializeRandom() const;
	inline int randomInt(const int max) const;
	inline QDate randomDate(const int minimumYear = 1930) const;

	inline Name randomMaleName() const;
	inline Name randomFemaleName() const;
	inline QString randomStreetname() const;



	// Dummy данные.
	QStringList m_maleFirstName;
	QStringList m_maleSurname;
	QStringList m_malePatronymic;

	QStringList m_femaleFirstName;
	QStringList m_femaleSurname;
	QStringList m_femalePatronymic;

	QStringList m_streetName;

	QStringList m_departments;

private slots:
	void updateInformation();
	void createPatients();
	void createStaff();

};
