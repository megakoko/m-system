#include "maindummydatabasewidget.h"

#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


#include "macros.h"
#include "dummydatabase.h"

#include "dummypatients.h"


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
	DummyPatients p(m_dummyData);
	p.createPatients(patientsCount);

	updateInformation();
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




