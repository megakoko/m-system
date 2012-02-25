#pragma once

#include <QObject>
#include "examwidgetfactory.h"

class QWidget;
class QLabel;
class QPushButton;


// Базовый класс для всех виджетов интерфейса первичного осмотра.
class ExamWidget : public QObject
{
	Q_OBJECT
	friend class ExamWidgetFactory;
public:
	ExamWidget(const int examId, const QString& textid);
	virtual ~ExamWidget();

	static const int InvalidId = 0;

	// Возвращает true, если значение, которое хранит виджет, НЕ отличается от значения
	// по-умолчанию.
	virtual bool valueIsNull() const = 0;

	// Возвращает true, если значение, хранящееся в виджете может быть сброшено.
	virtual bool valueCanBeReseted() const;

	// Возвращает текущее значение, которое хранит виджет.
	virtual QString value() const = 0;

	// Инициализирует виджет данными из базы данных для осмотра с идентификатором examId.
	virtual void init() = 0;

	// Сохраняет данные, введенные в виджете и возращает true, если сохранение прошло
	// успешно.
	virtual bool save(const int examId) = 0;

	// Возвращает виджет с надписью QLabel. Если такой виджет не нужен, можно
	// возвращать NULL,
	virtual QLabel* label() const = 0;

	// Возвращает виджет, с которым будет взаимодействовать пользователь.
	virtual QWidget* widget() const = 0;

	// Возвращает название виджета.
	QString labelText() const;

	//
	QString shortLabelText() const;

	void setUiElementId(const int id);

public slots:
	// Сбрасывает значение, которое хранит виджет.
	virtual void resetValue();

protected:
	static ExamWidgetFactory m_factory;

	// Поле таблицы Examination::id.
	int m_examId;

	// Поле таблицы ExaminationData::id.
	int m_examDataId;

	// Поле таблицы UiElement::id.
	int m_uielementId;

	// Текстовый идентификатор виджета (берется из базы данных).
	const QString m_textid;

	virtual void setLabelText(const QString& labelText, const QString &shortLabelText);

private:
	QString m_labelText;
	QString m_shortLabelText;

signals:
	// Сигнал, который должен испускаться при изменении значения виджета.
	// Параметр типа bool равен true, если значение является значением по-умолчанию.
	void valueChanged(bool isNull);

};

