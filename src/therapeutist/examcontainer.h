#pragma once

#include "examwidget.h"


class QLabel;
class QPushButton;
class QGridLayout;


/*
	Контейнер для других ExamWidget'ов.
*/
class ExamContainer : public ExamWidget
{
	Q_OBJECT
public:
	ExamContainer(const int examId, const QString& textid, const QString& labelText);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId) const;

	QLabel* label() const;
	QWidget* widget() const;

private:
	// Виджет, который содержит заголовок контейнера и контейнер.
	QWidget* m_widget;

	// Заголовок контейнера.
	QPushButton* m_header;
	// Собственно сам контейнер, виджет, который будет хранить в себе другие виджеты.
	QWidget* m_container;

	// Layout для контейнера.
	QGridLayout* m_containerLayout;


	// Список виджетов, которых содержит контейнер.
	QList<ExamWidget*> m_items;

private slots:
	void expandContainer(bool expanded);

};
