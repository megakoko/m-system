#include "dummyexaminations.h"

#include <QApplication>
#include <QProgressDialog>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"
#include "dummydatabase.h"


DummyExaminations::DummyExaminations(DummyDataPtr dummyData)
	: m_comboboxes(examinationComboBoxes())
	, m_containers(examinationContainers())
	, m_dummyData(dummyData)
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


QList<ComboBox> DummyExaminations::randomComboBoxes(const int percentage)
{
	Q_ASSERT(percentage <= 100 && percentage >= 0);
	const int requiredSize = m_comboboxes.size() * percentage / 100.0;


	// Проще выкинуть из list1 некоторые данные и вернуть list1.
	if(percentage > 50)
	{
		QList<ComboBox> list1(m_comboboxes);
		while(list1.size() >= requiredSize)
			list1.removeAt(m_dummyData->randomInt(list1.size()));

		return list1;
	}
	// Проще заполнить list2 данными из list1 и вернуть list2.
	else /* percentage <= 50 */
	{
		QList<ComboBox> list1(m_comboboxes);
		QList<ComboBox> list2;
		while(list2.size() <= requiredSize)
			list2 << list1.takeAt(m_dummyData->randomInt(list1.size()));

		return list2;
	}
}


void DummyExaminations::createExaminations(const int count, const int percentage)
{
	QProgressDialog progress("Создание первичных осмотров", "Остановить", 0, count);
	progress.setWindowModality(Qt::WindowModal);


	QSqlQuery q("BEGIN");
	for(int i = 0; i < count; ++i)
	{
		if(progress.wasCanceled())
			break;
		else
		{
			progress.setValue(i);
			qApp->processEvents();
		}


		const QVariant& patientId = m_dummyData->randomPatientId();
		const QVariant& therapeutistId = m_dummyData->randomTherapeutistId();

		if(patientId.isNull() || therapeutistId.isNull())
			break;


		q.prepare(" INSERT INTO Examination "
				  " (patientId, examinedByStaffId, examinationDate) "
				  " VALUES(?, ?, ?) " +
				  DummyDatabase::interfaces->db->returningSentence("id"));
		q.addBindValue(patientId);
		q.addBindValue(therapeutistId);


		static const int secondsInMonth = 30 * 24 * 60 * 60;
		QDateTime time(QDateTime::currentDateTime());
		time = time.addSecs(-1 * m_dummyData->randomInt(secondsInMonth));
		q.addBindValue(time);
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



		QList<ComboBox> comboboxes = randomComboBoxes(percentage);
		for(int i = 0; i < comboboxes.size(); ++i)
		{
			ComboBox c = comboboxes.at(i);

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
			if(m_containers.contains(containerIds.at(i).toInt()))
			{
				const int parentContainterId = m_containers.value(containerIds.at(i).toInt());
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

	progress.setValue(count);
	q.exec("COMMIT");
}
