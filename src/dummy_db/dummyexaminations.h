#pragma once

#include "dummydata.h"


struct ComboBox {
	int id;
	int parentId;
	QList<int> items;
};


class DummyExaminations
{
public:
	DummyExaminations(DummyDataPtr dummyData);
	void createExaminations(const int count, const int percentage);

private:
	//
	QList<ComboBox> examinationComboBoxes() const;
	/// id -> parentId
	QMap<int, int> examinationContainers() const;

	QList<ComboBox> randomComboBoxes(const int percentage);

	const QList<ComboBox> m_comboboxes;
	const QMap<int, int> m_containers;

	DummyDataPtr m_dummyData;
};
