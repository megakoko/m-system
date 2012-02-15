#pragma once


#include "dummydata.h"

class DummyDepartments
{
public:
	DummyDepartments(DummyDataPtr dummyData);

	void createStaff(const int count);
	void createDepartments();

private:
	/// Возвращает идентификаторы всех должностей из базы данных.
	QVariantList positionIdsFromDatabase() const;
	/// Возвращает идентификаторы всех сотрудников из базы данных.
	QVariantList staffIdsFromDatabase() const;


	/// Создает тестовые должности в БД.
	void createPositions();
	/// Создает записи в таблице DepartmentStaffPosition.
	/// (то есть заполняет отделение \a depaertmentId случайными сотрудниками из списка
	/// \a staffIds с должностями из списка \a staffIds.
	void createDepartmentStaffPositions(const QVariant departmentId,
										const QVariant headOfDepartmentId,
										const QVariantList& staffIds,
										const QVariantList& positionIds);

	/// Возвращает сокращенное название отделения.
	static QString departmentShortName(const QString& departmentFullName);

	DummyDataPtr m_dummyData;

};
