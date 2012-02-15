#include "dummydepartments.h"

#include <QApplication>
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
	QProgressDialog progress("Создание персонала", "Остановить", 0, count);
	progress.setWindowModality(Qt::WindowModal);


	QSqlQuery q("BEGIN");
	q.prepare(" INSERT INTO Staff "
			  " (familyName, name, patronymic, birthday, specialization) "
			  " VALUES (?, ?, ?, ?, ?)");


	for(int i = 0; i < count; ++i)
	{
		if(progress.wasCanceled())
			break;
		else
		{
			progress.setValue(i);
			qApp->processEvents();
		}

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


	progress.setValue(count);
	q.exec("COMMIT");
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



	QSqlQuery q("BEGIN");

	q.exec("SELECT name FROM Department");
	checkQuery(q);

	QSet<QString> departmentsInDatabase;
	while(q.next())
		departmentsInDatabase << q.value(0).toString();

	const QStringList& departments = m_dummyData->departments();


	QProgressDialog progress("Создание отделений", "Остановить", 0, departments.size());
	progress.setWindowModality(Qt::WindowModal);


	q.prepare(" INSERT INTO Department "
			  " (name, shortName, typeid, headOfDepartmentId) "
			  " VALUES(?, ?, (SELECT id FROM DepartmentType WHERE textid = ?), ?)" +
			  DummyDatabase::interfaces->db->returningSentence("id"));
	for(int i = 0; i < departments.size(); ++i)
	{
		if(progress.wasCanceled())
			break;
		else
		{
			progress.setValue(i);
			qApp->processEvents();
		}

		const QString& name = departments.at(i);
		if(!departmentsInDatabase.contains(name) && !name.isEmpty())
		{
			const QString& type = m_dummyData->randomInt(2) ? "clinic" : "hospital";
			const QVariant headId = staffIds.at(m_dummyData->randomInt(staffIds.size()));


			q.addBindValue(name);
			q.addBindValue(departmentShortName(name));
			q.addBindValue(type);
			q.addBindValue(headId);

			q.exec();
			checkQuery(q);

			const QVariant& departmentId = DummyDatabase::interfaces->db->lastInsertedId(&q);
			createDepartmentStaffPositions(departmentId, headId, staffIds, positionIds);
		}
	}

	progress.setValue(departments.size());
	q.exec("COMMIT");
}


QString DummyDepartments::departmentShortName(const QString &departmentFullName)
{
	const QStringList& words = departmentFullName.split(QRegExp("\\s+"), QString::SkipEmptyParts);

	QString shortName;
	if(!words.isEmpty())
	{
		// Первое слово - аббревиатура. Тогда его и возвращаем.
		if(words[0].toUpper() == words[0])
			shortName = words[0];
		else foreach(const QString& word, words)
			shortName += word[0].toUpper();
	}

	return shortName;
}
