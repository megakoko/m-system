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


namespace grouping {
static const QVariant none = 0;
static const QVariant byTherapeutist = 1;
}


bool sortByTherapeutist(const Examination& left, const Examination& right)
{
	return left.therapeutistName < right.therapeutistName;
}



ExaminationListReport::ExaminationListReport(QWidget *parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void ExaminationListReport::init()
{
	m_grouping->addItem("Без группировки", grouping::none);
	m_grouping->addItem("По врачам", grouping::byTherapeutist);


	populateExamList();
	createHtmlDocument();
}


void ExaminationListReport::initConnections()
{
	connect(m_grouping, SIGNAL(currentIndexChanged(int)), SLOT(createHtmlDocument()));
}


bool ExaminationListReport::groupedByTherapeutistName() const
{
	return m_grouping->itemData(m_grouping->currentIndex()) == grouping::byTherapeutist;
}



void ExaminationListReport::createHtmlDocument()
{
	m_doc.clear();
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



	if(groupedByTherapeutistName())
	{
		QList<Examination> examList = m_examList;
		qSort(examList.begin(), examList.end(), sortByTherapeutist);


		int currentTherapeutistId = -1;
		foreach(const Examination& exam, examList)
		{
			if(exam.therapeutistId != currentTherapeutistId)
			{
				currentTherapeutistId = exam.therapeutistId;
				addTherapeutistHeader(exam, table);
				addTableHeader(table);
			}
			addRow(exam, table);
		}
	}
	else
	{
		addTableHeader(table);
		foreach(const Examination& exam, m_examList)
			addRow(exam, table);
	}




	m_view->setHtml(m_doc.toByteArray());

#ifndef QT_NO_DEBUG
	const QString& tmpLocation = QDesktopServices::storageLocation(
									 QDesktopServices::TempLocation);

	QFile f(QDir(tmpLocation).filePath("exam_report.html"));
	if(f.open(QIODevice::WriteOnly))
		f.write(m_doc.toByteArray(2));
#endif
}


void ExaminationListReport::addTableHeader(QDomElement &table)
{
	QDomElement row = addElement(table, "tr");
	addElement(row, "th", "Время осмотра").setAttribute("align", "left");
	addElement(row, "th", "Пациент").setAttribute("align", "left");
	if(!groupedByTherapeutistName())
		addElement(row, "th", "Врач").setAttribute("align", "left");
}


void ExaminationListReport::addTherapeutistHeader(const Examination &exam, QDomElement &table)
{
	QDomElement row = addElement(table, "tr");
	row.setAttribute("colspan", 3);
	addElement(row, "td", exam.therapeutistName).setAttribute("class", "therapeutist");
}


void ExaminationListReport::addRow(const Examination &exam, QDomElement &table)
{
	QDomElement row = addElement(table, "tr");
	addElement(row, "td", exam.time.toString("dd.MM.yyyy HH:mm"));
	addElement(row, "td", exam.patientName);
	if(!groupedByTherapeutistName())
		addElement(row, "td", exam.therapeutistName);
}


QString ExaminationListReport::css()
{
	return	"td.therapeutist {"
				"font-weight: bold;"
				"font-size: large;"
			"}";
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
