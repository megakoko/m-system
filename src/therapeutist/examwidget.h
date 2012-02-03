#pragma once

#include <QWidget>
#include "examwidgetfactory.h"

// Базовый класс для всех виджетов интерфейса первичного осмотра.
class ExamWidget : public QWidget
{
	friend class ExamWidgetFactory;
public:
	ExamWidget(const int examId, const QString& textid, const QString &label);
	virtual ~ExamWidget();

	static const int InvalidId = 0;

	// Возвращает true, если значение, которое хранит виджет, НЕ отличается от значения
	// по-умолчанию.
	virtual bool valueIsNull() const = 0;

	// Возвращает текущее значение, которое хранит виджет.
	virtual QString value() const = 0;

	// Инициализирует виджет данными из базы данных для осмотра с идентификатором examId.
	virtual void init() = 0;

	// Сохраняет данные, введенные в виджете и возращает true, если сохранение прошло
	// успешно.
	virtual bool save(const int examId) const = 0;

	// Возвращает название виджета.
	QString label() const;

protected:
	static ExamWidgetFactory m_factory;

	// Поле таблицы Examination::id.
	int m_examId;

	// Поле таблицы ExaminationData::id.
	int m_examDataId;


	// Идентификатор виджета (берется из базы данных).
	const QString m_textid;

	void setLabel(const QString& label);
	QString m_label;

private:

};

