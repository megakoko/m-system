#include "dummydepartments.h"

#include <QProgressDialog>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"
#include "dummydatabase.h"



DummyDepartments::DummyDepartments(DummyDataPtr dummyData)
	: m_dummyData(dummyData)
{

}


void DummyDepartments::createStaff(const int count)
{
	QSqlQuery q;
	q.prepare(" INSERT INTO Staff "
			  " (familyName, name, patronymic, birthday, specialization) "
			  " VALUES (?, ?, ?, ?, ?)");


	for(int i = 0; i < count; ++i)
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
}


QVariantList DummyDepartments::positionIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Position");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


QVariantList DummyDepartments::staffIdsFromDatabase() const
{
	QSqlQuery q("SELECT id FROM Staff");
	checkQuery(q);

	QVariantList ids;
	while(q.next())
		ids << q.value(0);

	return ids;
}


void DummyDepartments::createPositions()
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


void DummyDepartments::createDepartmentStaffPositions(const QVariant departmentId,
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


void DummyDepartments::createDepartments()
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
