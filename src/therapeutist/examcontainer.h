#pragma once

#include "examwidget.h"


class QLabel;
class QPushButton;


/*
	Контейнер для других ExamWidget'ов.
*/
class ExamContainer : public ExamWidget
{
	Q_OBJECT
public:
	ExamContainer(const int examId, const QString& textid, const QString& label);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

private:
//	void



	// Заголовок контейнера.
	QPushButton* m_header;
	// Собственно сам контейнер, виджет, который будет хранить в себе другие виджеты.
	QWidget* m_container;
	// "Нижний колонтитул"; место, где будет показываться информация об элементах,
	// данные в которых отличаются от данных по-умолчанию.
	QLabel* m_footer;


//	bool m_initializedChildren;

	// Список виджетов, которых содержит контейнер.
	QList<ExamWidget*> m_items;

private slots:
	void expandContainer(bool expanded);

};
