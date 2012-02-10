#include "examinationpreview.h"

#include <QDateTime>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

// Для отладки.
#ifndef QT_NO_DEBUG
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#endif

#include "therapeutist.h"
#include "macros.h"


QDomElement addElement(QDomElement &parent,
					   const QString &tagName,
					   const QString &text = QString::null)
{
	Q_ASSERT(!parent.isNull());
	QDomDocument doc = parent.ownerDocument();

	QDomElement element = doc.createElement(tagName);
	parent.appendChild(element);
	if(!text.isNull())
		element.appendChild(doc.createTextNode(text));

	return element;
}


QString decode(const QString& cipherText)
{
	return Therapeutist::interfaces->enc->decode(cipherText);
}


const QString ExaminationPreview::CSS =
		"";


ExaminationPreview::ExaminationPreview(const int examinationId, QWidget *parent)
	: PluginWidget(parent)
	, m_examId(examinationId)
{
	setupUi(this);

	init();
}


void ExaminationPreview::init()
{
	QDomElement html = m_doc.createElement("html");
	m_doc.appendChild(html);

	QDomElement head = addElement(html, "head");
	QDomElement body = addElement(html, "body");


	// Задаем кодировку.
	QDomElement meta = addElement(head, "meta");
	meta.setAttribute("http-equiv", "Content-Type");
	meta.setAttribute("content", "text/html; charset=utf-8");

	QDomElement style = addElement(head, "style", CSS);
	style.setAttribute("type", "text/css");


	addElement(body, "h1", "Первичный осмотр пациента");
	addExaminationInformation(body);
	addExaminationData(body);



	m_webView->setHtml(m_doc.toByteArray());


#ifndef QT_NO_DEBUG
	const QString& tmpLocation = QDesktopServices::storageLocation(
									 QDesktopServices::TempLocation);

	QFile f(QDir(tmpLocation).filePath("exam_preview.html"));
	if(f.open(QIODevice::WriteOnly))
		f.write(m_doc.toByteArray(2));
#endif
}


void ExaminationPreview::addExaminationInformation(QDomElement &body)
{
	QSqlQuery q;
	q.prepare(" SELECT e.examinationDate, p.familyName, p.name, p.patronymic, p.birthday "
			  " FROM Examination e "
			  " JOIN Patient p ON e.patientId = p.id "
			  " WHERE e.id = ? ");
	q.addBindValue(m_examId);
	q.exec();
	checkQuery(q);

	if(q.first())
	{
		const QString& examDate = q.value(0).toDateTime().toString("dd/MM/yyyy HH:mm");
		addElement(body, "p", "Дата осмотра: " + examDate + ".");


		addElement(body, "p", "Пациент: " + decode(q.value(1).toString()) +
							  " " + decode(q.value(2).toString()) +
							  " " + decode(q.value(3).toString()) + ".");

		const QString& birthDay = q.value(4).toDate().toString("dd/MM/yyyy");
		addElement(body, "p", "Дата рождения: " + birthDay + ".");
	}
}


QString ExaminationPreview::examDataQuery()
{
	return	" SELECT ui.label, d.textValue, d.realValue, uiEnum.value, "
				" ui.typeid, ui.textId "
			" FROM ExaminationData d "
			" LEFT JOIN UiElement ui ON d.uielementId = ui.id "
			" LEFT JOIN UiElementEnums uiEnum ON "
				" (ui.textid = uiEnum.uiElementTextId AND d.enumValue = uiEnum.id) "
			" WHERE (d.examinationId = ? AND ui.parentId = ?) "
			" ORDER BY ui.id ";
}


void ExaminationPreview::addExaminationData(QDomElement &body)
{
	QSqlQuery q;
	q.prepare(examDataQuery());
	q.addBindValue(m_examId);
	q.addBindValue("main");
	q.exec();
	checkQuery(q);


	while(q.next())
	{
		if(q.value(4).toString() == "container")
		{
			const QString& containerLabel = q.value(0).toString();
			QStringList dataList = containerData(q.value(5).toString());

			addElement(body, "h2", containerLabel);
			addElement(body, "p", dataList.join(". ").append("."));
		}
		else
		{
			QString data = examDataFromSqlRecord(q.record());

			addElement(body, "p", data.append(". "));
		}
	}
}


QString ExaminationPreview::examDataFromSqlRecord(const QSqlRecord &record)
{
	const QString& type = record.value(4).toString();
	Q_ASSERT(!type.isNull() && type != "container");


	QString data = record.value(0).toString();

	bool recordHasValue = false;
	for(int recordColumn = 1; recordColumn <= 3; ++recordColumn)
		if(!record.value(recordColumn).isNull())
		{
			data += ": " + record.value(recordColumn).toString();
			recordHasValue = true;
			break;
		}
	Q_ASSERT(recordHasValue);

	return data;
}


QStringList ExaminationPreview::containerData(const QString& containerTextId) const
{
	QStringList dataList;

	QSqlQuery q;
	q.prepare(examDataQuery());

	q.addBindValue(m_examId);
	q.addBindValue(containerTextId);
	q.exec();
	checkQuery(q);


	while(q.next())
	{
		if(q.value(4).toString() == "container")
			dataList << containerData(q.value(5).toString());
		else
			dataList << examDataFromSqlRecord(q.record());
	}

	return dataList;
}