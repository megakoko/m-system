#pragma once

#include "ui_daterangedialog.h"

class DateRangeDialog : public QDialog, private Ui::DateRangeDialog
{
	Q_OBJECT
public:
	explicit DateRangeDialog(const QDate& start, const QDate& end,
							 const QList<QDate>& dates, QWidget *parent = 0);

	QDate startDate() const;
	QDate endDate() const;

private:
	void init(const QDate& start, const QDate& end);
	void initConnections();

	void updateExaminationLabel(const int examinationCount);

	QList<QDate> m_dates;

private slots:
	void dateChanged();

};
