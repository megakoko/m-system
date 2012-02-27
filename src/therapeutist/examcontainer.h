#pragma once

#include "examwidget.h"


class QLabel;
class QGridLayout;
class ExaminationEditWidget;


/*
	Контейнер для других ExamWidget'ов.
*/
class ExamContainer : public ExamWidget
{
	Q_OBJECT
public:
	ExamContainer(const int examId, const QString& textid, const bool topLevel = false);

	void setExaminationEditWidget(ExaminationEditWidget* widget);

	// Определенные чисто виртуальные функции.
	bool valueIsNull() const;
	QString value() const;
	void init();
	bool save(const int examId);
	QMap<int, QVariant> data() const;

	QLabel* label() const;
	QWidget* widget() const;

protected:
	void setLabelText(const QString &labelText, const QString& shortLabelText);

private:
	// Возвращает значение всех элементов, содержащихся в контейнере
	// с textid = containerTextId.
	QStringList containerValueFromDatabase(const QString& containerTextId) const;
	// То же самое, что и containerValueFromDatabase, только возвращает данные в виде
	// карты QMap.
	QMap<int, QVariant> containerDataFromDatabase(const QString& containerTextId) const;

	void expandContainer(const bool expanded);


	static const int m_indicatorWidth;
	static const QString labelAndValueDelimiter;

	const bool m_topLevel;

	// Виджет, который содержит заголовок контейнера и контейнер.
	QWidget* m_widget;

	// Индикатор "свернутости" в заголовке контейнера.
	QLabel* m_headerIndicator;

	// Текст в заголовоке контейнера.
	QLabel* m_headerText;

	// Собственно сам контейнер, виджет, который будет хранить в себе другие виджеты.
	QWidget* m_container;

	// Layout для контейнера.
	QGridLayout* m_containerLayout;

	// Список виджетов, которых содержит контейнер.
	QList<ExamWidget*> m_items;

	// Указатель на главный виджет осмотра. Его мы спросим о возрасте пациента.
	ExaminationEditWidget* m_examinationEditWidget;

private slots:
	void updateHeader();

	void expandContainer();

};
