#pragma once

#include "ui_examinationlistreport.h"
#include "pluginwidget.h"

#include <QDateTime>
#include <QDomDocument>


struct Examination
{
	QDateTime time;
	QString patientName;
	int therapeutistId;
	QString therapeutistName;
};


class ExaminationListReport : public PluginWidget, private Ui::ExaminationListReport
{
	Q_OBJECT
public:
	explicit ExaminationListReport(QWidget *parent = 0);

private:
	void init();
	void initConnections();


	bool groupedByTherapeutistName() const;
	bool groupedByDate() const;

	void addTherapeutistHeader(const Examination& exam, QDomElement& table);
	void addDateHeader(const Examination& exam, QDomElement& table);
	void addTableHeader(QDomElement& table);
	void addEmptyRow(QDomElement& table);
	void addRow(const Examination& exam, QDomElement& table);

	static QString css();

	void populateExamList();

	QList<Examination> m_examList;

	QDomDocument m_doc;

	QDate m_startDate;
	QDate m_endDate;

private slots:
	void createHtmlDocument();
	void chooseDateRange();
	void updateStyle();
	void print();

};
