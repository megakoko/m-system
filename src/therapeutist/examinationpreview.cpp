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


	QDomElement style = addElement(head, "style", CSS);
	style.setAttribute("type", "text/css");


	addElement(body, "h1", "Первичный осмотр пациента");
	addExaminationInformation(body);



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
		addElement(body, "p", "Дата осмотра: " + examDate);


		addElement(body, "p", "Пациент: " + decode(q.value(1).toString()) +
							  " " + decode(q.value(2).toString()) +
							  " " + decode(q.value(3).toString()));

		const QString& birthDay = q.value(4).toDate().toString("dd/MM/yyyy");
		addElement(body, "p", "Дата рождения: " + birthDay);
	}
}
