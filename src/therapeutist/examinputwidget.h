#pragma once

#include "examwidget.h"

#include <QVariant>

class ExamInputWidget : public ExamWidget
{
	Q_OBJECT
public:
	ExamInputWidget(const int examId, const QString& textId);

	// Возвращает надпись m_label.
	QLabel* label() const;

public slots:
	// Обновляет стиль надписи: делает ее полужирной, если значение виджета не null.
	void updateLabelStyle();

protected:
	void setLabelText(const QString &labelText, const QString &shortLabelText);

	// Помогает в инициализации виджета: возвращает значение, которое хранится в поле
	// fieldName таблицы ExaminationData.
	QVariant initHelpher(const QString& fieldName);

	// Помогает в сохранении значения, введенного в виджет (value): сохраняет его в поле
	// fieldName таблицы ExaminationData с внешним ключом на таблицу Examination - examId.
	bool saveHelper(const QString& fieldName, const int examId, const QVariant &value) const;

private:
	// Надпись.
	QLabel* m_label;

};
