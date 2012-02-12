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
	void createExaminations(const int count);

private:
	//
	QList<ComboBox> examinationComboBoxes() const;
	/// id -> parentId
	QMap<int, int> examinationContainers() const;

	DummyDataPtr m_dummyData;
};
