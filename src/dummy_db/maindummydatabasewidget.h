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
