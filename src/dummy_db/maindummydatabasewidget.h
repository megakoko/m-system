#pragma once


#include "pluginwidget.h"
#include "ui_maindummydatabasewidget.h"

#include <QSharedPointer>
#include "dummydata.h"


struct ComboBox {
	int id;
	int parentId;
	QList<int> items;
};



class MainDummyDatabaseWidget : public PluginWidget, private Ui::MainDummyDatabaseWidget
{
	Q_OBJECT

public:
	explicit MainDummyDatabaseWidget(QWidget *parent = 0);


private:
	void init();
	void initConnections();


	// Вставка в различные таблицы.

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


	//
	QList<ComboBox> examinationComboBoxes() const;
	/// id -> parentId
	QMap<int, int> examinationContainers() const;


	QSharedPointer<DummyData> m_dummyData;

private slots:
	void updateInformation();
	void createPatients();
	void createStaff();
	void createDepartments();
	void createExaminations();

};
