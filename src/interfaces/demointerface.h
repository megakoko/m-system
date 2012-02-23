#pragma once

/// Интерфейс информации о демо-версии.
class DemoInterface
{
public:
	/// Деструктор.
	virtual ~DemoInterface() {}

	/// Возвращает true, если используется режим демо-версия.
	virtual bool isDemoVersion() const = 0;

	/// Возвращает максимальное количество пациентов в демо-версии.
	virtual int patientCountLimit() const = 0;

	/// Возвращает максимальное количество осмотров в демо-версии.
	virtual int examinationCountLimit() const = 0;

};
