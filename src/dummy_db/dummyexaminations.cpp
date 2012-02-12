#include "dummyexaminations.h"

#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"
#include "dummydatabase.h"


DummyExaminations::DummyExaminations(DummyDataPtr dummyData)
	: m_dummyData(dummyData)
{

}


QList<ComboBox> DummyExaminations::examinationComboBoxes() const
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


QMap<int, int> DummyExaminations::examinationContainers() const
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


void DummyExaminations::createExaminations(const int count)
{
	const QList<ComboBox>& comboboxes = examinationComboBoxes();
	const QMap<int, int>& containers = examinationContainers();



	for(int i = 0; i < count; ++i)
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
