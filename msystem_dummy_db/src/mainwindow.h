#pragma once

#include "ui_mainwindow.h"

class QFile;


class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);


private:
	void init();
	void initConnections();

	void loadFiles();
	void loadFile(const QString& fileName, QStringList& stringList);
	bool tryToOpen(QFile& f) const;
	void readFile(QFile& f, QStringList& stringList);

	void showMessageInStatusBar(const QString& msg);


	// Вставка в различные таблицы.

	// Возвращает ID пациента.
	int createPatientRecord() const;
	void createDocumentRecord(const int patientId, const QString& documentTextid) const;
	void createAddressRecord(const int patientId, const QString& addressType) const;


	// Random-функции.
	void initializeRandom() const;
	inline int randomInt(const int max) const;
	inline QDate randomDate() const;

	inline void randomMaleName(QString& surname, QString& firstname,
							   QString& patronymic) const;
	inline void randomFemaleName(QString& surname, QString& firstname,
								 QString& patronymic) const;
	inline QString randomStreetname() const;


	// Dummy данные.
	QStringList m_maleFirstName;
	QStringList m_maleSurname;
	QStringList m_malePatronymic;

	QStringList m_femaleFirstName;
	QStringList m_femaleSurname;
	QStringList m_femalePatronymic;

	QStringList m_streetName;

private slots:
	void reconnectToDb();

	void updatePatientsCount();
	void createPatients();

};
