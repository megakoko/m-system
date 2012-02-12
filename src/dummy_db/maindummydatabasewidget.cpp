#include "maindummydatabasewidget.h"

#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QProgressDialog>


#include "macros.h"
#include "dummydatabase.h"


MainDummyDatabaseWidget::MainDummyDatabaseWidget(QWidget *parent)
	: PluginWidget(parent)
	, m_dummyData(new DummyData)
{
	setupUi(this);

	init();
	initConnections();
}


void MainDummyDatabaseWidget::init()
{
	updateInformation();
}


void MainDummyDatabaseWidget::initConnections()
{
	connect(m_updateInformation, SIGNAL(clicked()), SLOT(updateInformation()));
	connect(m_createPatients, SIGNAL(clicked()), SLOT(createPatients()));
	connect(m_createStaff, SIGNAL(clicked()), SLOT(createStaff()));
	connect(m_createDepartments, SIGNAL(clicked()), SLOT(createDepartments()));
	connect(m_createExaminations, SIGNAL(clicked()), SLOT(createExaminations()));
}


QString MainDummyDatabaseWidget::encode(const QString &plaintext) const
{
	return DummyDatabase::interfaces->enc->encodeStr(plaintext);
}


QString MainDummyDatabaseWidget::encode(const QDate &plainDate) const
{
	return DummyDatabase::interfaces->enc->encodeDate(plainDate);
}


void MainDummyDatabaseWidget::updateInformation()
{
	QSqlQuery q("SELECT COUNT(*) FROM Patient");
	checkQuery(q);

	q.first();
	m_patientsCount->setNum(q.value(0).toInt());

	q.exec("SELECT COUNT(*) FROM Staff");
	checkQuery(q);
	q.first();
	m_staffCount->setNum(q.value(0).toInt());
}


void MainDummyDatabaseWidget::createPatients()
{
	const int patientsCount = m_createPatientsCount->value();

	QProgressDialog progress("Создание пациентов", "Остановить", 0, patientsCount, this);
	progress.setWindowModality(Qt::WindowModal);


	QSqlQuery q("BEGIN");
	for(int i = 0; i < patientsCount; ++i)
	{
		if(progress.wasCanceled())
			break;

		if(i % 100 == 0)
		{
			progress.setValue(i);
			qApp->processEvents();
		}


		const QDate& birthday = m_dummyData->randomDate();
		const int patientId = createPatientRecord(birthday);

		createDocumentRecord(patientId, "passport", birthday);
		createDocumentRecord(patientId, "insuranceMandatory", birthday);
		if(m_dummyData->randomInt(2))
			createDocumentRecord(patientId, "insuranceVoluntary", birthday);


		createAddressRecord(patientId, "mailing");
		if(m_dummyData->randomInt(5) == 0)
			createAddressRecord(patientId, "actual");


	}
	progress.setValue(patientsCount);

	q.exec("COMMIT");
	updateInformation();
}


int MainDummyDatabaseWidget::createPatientRecord(const QDate &birthday) const
{
	Name patientName;

	QString sextextid;
	if(m_dummyData->randomInt(2))
	{
		patientName = m_dummyData->randomMaleName();
		sextextid = "male";
	}
	else
	{
		patientName = m_dummyData->randomFemaleName();
		sextextid = "female";
	}


	QSqlQuery q;
	q.prepare("INSERT INTO Patient(familyname, name, patronymic, birthday, sexid) VALUES"
			  "(:familyname, :name, :patronymic, :birthday, "
				" (SELECT id FROM Sex WHERE textid = :sextextid)) " +
			  DummyDatabase::interfaces->db->returningSentence("id"));
	q.bindValue(":familyname", encode(patientName.surname));
	q.bindValue(":name", encode(patientName.firstname));
	q.bindValue(":patronymic", encode(patientName.patronymic));
	q.bindValue(":birthday", encode(birthday));
	q.bindValue(":sextextid", sextextid);
	q.exec();
	checkQuery(q);


	q.first();
	return DummyDatabase::interfaces->db->lastInsertedId(&q).toInt();
}


void MainDummyDatabaseWidget::createDocumentRecord(const int patientId,
												   const QString& documentTextid,
												   const QDate &patientBirthday) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Document "
			  " (documentTypeId, patientId, serialNumber, date, givenBy) VALUES "
			  " ((SELECT id FROM DocumentType WHERE textid = :docTextid), "
			  " :patientId, :serialNumber, :date, :givenBy)");
	q.bindValue(":docTextid", documentTextid);
	q.bindValue(":patientId", patientId);
	q.bindValue(":serialNumber", encode(QString::number(m_dummyData->randomInt(1E4)) + ' ' +
										QString::number(m_dummyData->randomInt(1E6))));
	q.bindValue(":date", encode(m_dummyData->randomDate(patientBirthday.year() + 14)));
	if(documentTextid == "passport")
		q.bindValue(":givenBy", encode("РОВД"));
	else
		q.bindValue(":givenBy", encode("Страховая компания"));
	q.exec();
	checkQuery(q);
}


void MainDummyDatabaseWidget::createAddressRecord(const int patientId,
									 const QString& addressType) const
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Address "
			  " (patientId, city, street, house, apartment, typeId) VALUES"
			  " (:patientId, :city, :street, :house, :apartment, "
				" (SELECT id FROM AddressType WHERE textid = :textid))");
	q.bindValue(":patientId", patientId);
	q.bindValue(":textid", addressType);
	q.bindValue(":city", encode("Ижевск"));
	q.bindValue(":street", encode(m_dummyData->randomStreetname()));
	q.bindValue(":house", encode(QString::number(m_dummyData->randomInt(250))));
	q.bindValue(":apartment", encode(QString::number(m_dummyData->randomInt(200))));
	q.exec();
	checkQuery(q);
}


void MainDummyDatabaseWidget::createStaff()
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Staff "
			  " (familyName, name, patronymic, birthday, specialization) "
			  " VALUES (?, ?, ?, ?, ?)");


	for(int i = 0; i < m_createStaffCount->value(); ++i)
	{
		const Name& name = m_dummyData->randomInt(2) ?
						   m_dummyData->randomMaleName() :
						   m_dummyData->randomFemaleName();
		const QDate& birthday = m_dummyData->randomDate();
		const QString& specialization = "";

		q.addBindValue(name.surname);
		q.addBindValue(name.firstname);
		q.addBindValue(name.patronymic);
		q.addBindValue(birthday);
		q.addBindValue(specialization);
		q.exec();
		checkQuery(q);
	}

	updateInformation();
}


QVariantList MainDummyDatabaseWidget::positionIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Position");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


QVariantList MainDummyDatabaseWidget::staffIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Staff");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


void MainDummyDatabaseWidget::createPositions()
{
	static const QStringList predefinedPositions =
			QStringList() << "Врач" << "Лаборант" << "Медсестра";

	QSet<QString> positionsInDatabase;

	QSqlQuery q;

	q.exec("SELECT name FROM Position");
	checkQuery(q);
	while(q.next())
		positionsInDatabase << q.value(0).toString();


	q.prepare("INSERT INTO Position(name) VALUES(?)");
	foreach(const QString& position, predefinedPositions)
		if(!positionsInDatabase.contains(position))
		{
			q.addBindValue(position);
			q.exec();
			checkQuery(q);
		}
}


void MainDummyDatabaseWidget::createDepartmentStaffPositions(const QVariant departmentId,
															 const QVariant headOfDepartmentId,
															 const QVariantList &staffIds,
															 const QVariantList &positionIds)
{
	if(!staffIds.isEmpty() && !positionIds.isEmpty())
	{
		QSqlQuery q;
		q.prepare(" INSERT INTO DepartmentStaffPosition "
				  " (departmentId, staffId, positionId) "
				  " VALUES(?, ?, ?) ");


		const int numberOfPeopleInDepartment = staffIds.size() / 10;
		for(int i = 0; i < numberOfPeopleInDepartment - 1; ++i)
		{
			q.addBindValue(departmentId);
			q.addBindValue(staffIds.at(m_dummyData->randomInt(staffIds.size())));
			q.addBindValue(positionIds.at(m_dummyData->randomInt(positionIds.size())));
			q.exec();
			checkQuery(q);
		}


		// Глава отделения пусть тоже какую-то должность занимает помимо должности главы
		// отделения.
		q.addBindValue(departmentId);
		q.addBindValue(headOfDepartmentId);
		q.addBindValue(positionIds.at(m_dummyData->randomInt(positionIds.size())));
		q.exec();
		checkQuery(q);
	}
}


void MainDummyDatabaseWidget::createDepartments()
{
	createPositions();

	const QVariantList staffIds = staffIdsFromDatabase();
	const QVariantList positionIds = positionIdsFromDatabase();


	if(staffIds.isEmpty() || positionIds.isEmpty())
		return;


	QSqlQuery q;

	q.exec("SELECT name FROM Department");
	checkQuery(q);

	QSet<QString> departmentsInDatabase;
	while(q.next())
		departmentsInDatabase << q.value(0).toString();


	q.prepare(" INSERT INTO Department "
			  " (name, shortName, typeid, headOfDepartmentId) "
			  " VALUES(?, ?, (SELECT id FROM DepartmentType WHERE textid = ?), ?)" +
			  DummyDatabase::interfaces->db->returningSentence("id"));
	const QStringList& departments = m_dummyData->departments();
	for(int i = 0; i < departments.size(); ++i)
	{
		const QString& name = departments.at(i);
		if(!departmentsInDatabase.contains(name) && !name.isEmpty())
		{
			const QString& type = m_dummyData->randomInt(2) ? "clinic" : "hospital";
			const QVariant headId = staffIds.at(m_dummyData->randomInt(staffIds.size()));


			q.addBindValue(name);
			q.addBindValue(name[0].toUpper());
			q.addBindValue(type);
			q.addBindValue(headId);

			q.exec();
			checkQuery(q);

			const QVariant& departmentId = DummyDatabase::interfaces->db->lastInsertedId(&q);
			createDepartmentStaffPositions(departmentId, headId, staffIds, positionIds);
		}
	}
}



QList<ComboBox> MainDummyDatabaseWidget::examinationComboBoxes() const
{
	QSqlQuery q;
	q.exec(" SELECT ui.id, parent.id, e.id "
		   " FROM UiElement ui "
		   " LEFT JOIN UiElement parent ON ui.parentId = parent.textid "
		   " LEFT JOIN UiElementEnums e ON ui.textid = e.uielementtextid "
		   " WHERE ui.typeid = 'combobox' "
		   " ORDER BY ui.id");
	checkQuery(q);


	QList<ComboBox> comboBoxes;

	int id = -1;
	int parentId = -1;
	QList<int> items;
	while(q.next())
	{
		const int currentId = q.value(0).toInt();
		if(id == -1)
		{
			id = currentId;
			parentId = q.value(1).toInt();
		}


		// Новый combo box.
		if(currentId != id)
		{
			ComboBox c = {id, parentId, items};
			comboBoxes << c;

			id = currentId;
			parentId = q.value(1).toInt();
			items.clear();
		}

		items << q.value(2).toInt();
	}

	if(id != -1)
	{
		ComboBox c = {id, parentId, items};
		comboBoxes << c;
	}

	return comboBoxes;
}


QMap<int, int> MainDummyDatabaseWidget::examinationContainers() const
{
	QSqlQuery q;
	q.exec(" SELECT ui.id, parent.id "
		   " FROM UiElement ui "
		   " LEFT JOIN UiElement parent ON ui.parentId = parent.textid "
		   " WHERE ui.typeid = 'container' AND ui.parentid IS NOT NULL "
		   " ORDER BY ui.id");
	checkQuery(q);

	QMap<int, int> map;

	while(q.next())
		map[q.value(0).toInt()] = q.value(1).toInt();

	return map;
}


void MainDummyDatabaseWidget::createExaminations()
{
	const QList<ComboBox>& comboboxes = examinationComboBoxes();
	const QMap<int, int>& containers = examinationContainers();



	for(int i = 0; i < m_createExaminationsCount->value(); ++i)
	{
		QSqlQuery q;
		q.prepare(" INSERT INTO Examination "
				  " (patientId, examinationDate) "
				  " VALUES(?, ?) " +
				  DummyDatabase::interfaces->db->returningSentence("id"));	// TODO: examined staff.
		q.addBindValue(m_dummyData->randomPatientId());
		q.addBindValue(QDateTime::currentDateTime()); // TODO
		q.exec();
		checkQuery(q);

		const QVariant& examId = DummyDatabase::interfaces->db->lastInsertedId(&q);


		q.prepare(" INSERT INTO ExaminationData "
				  " (examinationId, uiElementId, enumValue) "
				  " VALUES(?, ?, ?) ");

		QVariantList examIds;
		QVariantList uiElementIds;
		QVariantList enumValueIds;

		QVariantList containerIds;

		for(int i = 0; i < comboboxes.size()/2; ++i)
		{
			ComboBox c = comboboxes.at(m_dummyData->randomInt(comboboxes.size()));

			examIds << examId;
			uiElementIds << c.id;
			enumValueIds << c.items.at(m_dummyData->randomInt(c.items.size()));

			if(!containerIds.contains(c.parentId))
				containerIds << c.parentId;
		}

		q.addBindValue(examIds);
		q.addBindValue(uiElementIds);
		q.addBindValue(enumValueIds);
		q.execBatch();
		checkQuery(q);

		q.prepare(" INSERT INTO ExaminationData "
				  " (examinationId, uiElementId) "
				  " VALUES(?, ?)");

		for(int i = 0; i < containerIds.count(); ++i)
		{
			if(containers.contains(containerIds.at(i).toInt()))
			{
				const int parentContainterId = containers.value(containerIds.at(i).toInt());
				if(!containerIds.contains(parentContainterId))
					containerIds << parentContainterId;
			}
		}

		examIds.clear();
		for(int i = 0; i < containerIds.count(); ++i)
			examIds << examId;


		q.addBindValue(examIds);
		q.addBindValue(containerIds);
		q.execBatch();
		checkQuery(q);
	}
}




