#include "daterangedialog.h"

DateRangeDialog::DateRangeDialog(const QDate &start, const QDate &end,
								 const QList<QDate> &dates, QWidget *parent)
	: QDialog(parent)
	, m_dates(dates)
{
	setupUi(this);

	init(start, end);
	initConnections();
}


void DateRangeDialog::init(const QDate &start, const QDate &end)
{
	if(!m_dates.isEmpty())
	{
		qSort(m_dates);

		m_startDateEdit->setMinimumDate(m_dates.first());
		m_startDateEdit->setMaximumDate(end);
		m_startDateEdit->setDate(start);

		m_endDateEdit->setMinimumDate(start);
		m_endDateEdit->setMaximumDate(m_dates.last());
		m_endDateEdit->setDate(end);
	}

	updateExaminationLabel(m_dates.size());
}


void DateRangeDialog::initConnections()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_startDateEdit, SIGNAL(dateChanged(QDate)), SLOT(dateChanged()));
	connect(m_endDateEdit, SIGNAL(dateChanged(QDate)), SLOT(dateChanged()));
}


void DateRangeDialog::dateChanged()
{
	const QDate& start = m_startDateEdit->date();
	const QDate& end = m_endDateEdit->date();

	m_startDateEdit->setMaximumDate(end);
	m_endDateEdit->setMinimumDate(start);


	// Вычисляем количество осмотров в выбранном периоде дат.
	int examinationCount = 0;
	foreach(const QDate& date, m_dates)
	{
		if(start <= date && date <= end)
			++examinationCount;
		else if(date > end)	// Можно дальше не идти по списку, т.к. он отсортирован.
			break;
	}

	updateExaminationLabel(examinationCount);
}


void DateRangeDialog::updateExaminationLabel(const int examinationCount)
{
	static const QString text = "Количество осмотров в периоде: %1";
	m_examinationCountLabel->setText(text.arg(examinationCount));
}


QDate DateRangeDialog::startDate() const
{
	return m_startDateEdit->date();
}


QDate DateRangeDialog::endDate() const
{
	return m_endDateEdit->date();
}
