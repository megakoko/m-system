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

	void addTherapeutistHeader(const Examination& exam, QDomElement& table);
	void addTableHeader(QDomElement& table);
	void addRow(const Examination& exam, QDomElement& table);

	static QString css();

	void populateExamList();

	QList<Examination> m_examList;

	QDomDocument m_doc;

private slots:
	void createHtmlDocument();

};
