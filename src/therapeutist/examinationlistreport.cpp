#include "examinationlistreport.h"

#include <QPrinter>
#include <QPrintDialog>

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
#include "daterangedialog.h"


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
	, m_startDate(QDate::currentDate())
	, m_endDate(QDate::currentDate())
{
	setupUi(this);

	init();
	initConnections();
}


void ExaminationListReport::init()
{
	m_grouping->addItem("Отсутствует", grouping::none);
	m_grouping->addItem("По врачам", grouping::byTherapeutist);

	m_fontFamily->addItem("С засечками", "'Times New Roman', Times, serif");
	m_fontFamily->addItem("Без засечек", "Arial, sans-serif");
	updateStyle();

	populateExamList();
	createHtmlDocument();
}


void ExaminationListReport::initConnections()
{
	connect(m_grouping, SIGNAL(currentIndexChanged(int)), SLOT(createHtmlDocument()));
	connect(m_choosePeriod, SIGNAL(clicked()), SLOT(chooseDateRange()));

	connect(m_fontFamily, SIGNAL(currentIndexChanged(int)), SLOT(updateStyle()));
	connect(m_fontSize, SIGNAL(valueChanged(int)), SLOT(updateStyle()));
	connect(m_print, SIGNAL(clicked()), SLOT(print()));
}


void ExaminationListReport::print()
{
	QPrinter printer;
	printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);
	QPrintDialog dialog(&printer, this);
	if(dialog.exec() == QDialog::Accepted)
		m_view->print(&printer);
}


void ExaminationListReport::updateStyle()
{
	const QVariant& family = m_fontFamily->itemData(m_fontFamily->currentIndex());
	const int size = m_fontSize->value();

	QFont f(family.toString(), size);
	m_view->document()->setDefaultFont(f);
}


void ExaminationListReport::chooseDateRange()
{
	QList<QDate> dates;
	foreach(const Examination& exam, m_examList)
		dates << exam.time.date();


	DateRangeDialog d(m_startDate, m_endDate, dates, this);
	if(d.exec() == QDialog::Accepted)
	{
		m_startDate = d.startDate();
		m_endDate = d.endDate();

		createHtmlDocument();
	}
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


	QDomElement h2 = addElement(body, "h2", "Список проведенных осмотров");
	addElement(h2, "h3", QString("(За период с %1 по %2)").
						 arg(m_startDate.toString("dd.MM.yyyy")).
						 arg(m_endDate.toString("dd.MM.yyyy")));


	QDomElement table = addElement(body, "table");
	table.setAttribute("width", "100%");



	if(groupedByTherapeutistName())
	{
		QList<Examination> examList = m_examList;
		qSort(examList.begin(), examList.end(), sortByTherapeutist);


		int currentTherapeutistId = -1;
		foreach(const Examination& exam, examList)
		{
			const QDate& date = exam.time.date();

			if(m_startDate <= date && date <= m_endDate)
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
	}
	else
	{
		addTableHeader(table);
		foreach(const Examination& exam, m_examList)
		{
			const QDate& date = exam.time.date();

			if(m_startDate <= date && date <= m_endDate)
				addRow(exam, table);
			else if(date < m_endDate)	// Осмотры отсортированы в порядке убывания даты.
				break;
		}
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


	if(q.first())
		m_endDate = q.value(0).toDate();
	if(q.last())
		m_startDate = q.value(0).toDate();
}
