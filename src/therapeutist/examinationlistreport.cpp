#include "examinationlistreport.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Для отладки.
#ifndef QT_NO_DEBUG
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#endif

#include "macros.h"
#include "domhelper.h"
#include "therapeutist.h"


ExaminationListReport::ExaminationListReport(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void ExaminationListReport::init()
{
	populateExamList();
	createHtmlDocument();


	m_view->setHtml(m_doc.toByteArray());
#ifndef QT_NO_DEBUG
	const QString& tmpLocation = QDesktopServices::storageLocation(
									 QDesktopServices::TempLocation);

	QFile f(QDir(tmpLocation).filePath("exam_report.html"));
	if(f.open(QIODevice::WriteOnly))
		f.write(m_doc.toByteArray(2));
#endif
}


void ExaminationListReport::initConnections()
{

}


void ExaminationListReport::createHtmlDocument()
{
	QDomElement html = m_doc.createElement("html");
	m_doc.appendChild(html);

	QDomElement head = addElement(html, "head");
	QDomElement body = addElement(html, "body");


	// Задаем кодировку.
	QDomElement meta = addElement(head, "meta");
	meta.setAttribute("http-equiv", "Content-Type");
	meta.setAttribute("content", "text/html; charset=utf-8");

	QDomElement style = addElement(head, "style", css());
	style.setAttribute("type", "text/css");



	QDomElement table = addElement(body, "table");
	table.setAttribute("width", "100%");
	addTableHeader(table);

	foreach(const Examination& exam, m_examList)
		addRow(exam, table);
}


void ExaminationListReport::addTableHeader(QDomElement &table)
{
	QDomElement row = addElement(table, "tr");
	addElement(row, "th", "Время осмотра");
	addElement(row, "th", "Пациент");
	addElement(row, "th", "Врач");
}


void ExaminationListReport::addRow(const Examination &exam, QDomElement &table)
{
	QDomElement row = addElement(table, "tr");
	addElement(row, "td", exam.time.toString("dd.MM.yyyy HH:mm"));
	addElement(row, "td", exam.patientName);
	addElement(row, "td", exam.therapeutistName);
}


QString ExaminationListReport::css()
{
	return QString::null;
}


void ExaminationListReport::populateExamList()
{
	QSqlQuery q(" SELECT ex.examinationDate, s.id, "
				" s.familyName || ' ' || s.name || ' ' || s.patronymic, "
				" p.familyName, p.name, p.patronymic "
				" FROM examination ex "
				" LEFT JOIN staff s ON ex.examinedbystaffid = s.id "
				" LEFT JOIN Patient p ON ex.patientId = p.id "
				" ORDER BY ex.examinationDate DESC ");

	checkQuery(q);


	while(q.next())
	{
		Examination e;
		e.time = q.value(0).toDateTime();
		e.therapeutistId = q.value(1).toInt();
		e.therapeutistName = q.value(2).toString();


		for(int col = 3; col <= 5; ++col)
		{
			e.patientName += Therapeutist::interfaces->enc->
							 decodeStr(q.value(col).toString()) + " ";
		}

		m_examList << e;
	}

}
