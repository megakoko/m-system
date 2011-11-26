#include "documenteditdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include "macros.h"



DocumentEditDialog::DocumentEditDialog(const Document& doc, QWidget *parent)
	: QDialog(parent)
	, m_document(doc)
{
    setupUi(this);


	QSqlQuery q("SELECT name, id FROM DocumentType ORDER BY id");
	while(q.next())
		m_documentType->addItem(q.value(0).toString(), q.value(1));
	m_documentType->setCurrentIndex(m_documentType->findData(doc.documentTypeId));

	m_serialNumber->setText(doc.serialNumber);
	m_number->setText(doc.number);
	m_date->setDate(doc.date);
	m_givenBy->setText(doc.givenBy);

	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));
}


Document DocumentEditDialog::document()
{
	m_document.documentTypeId =
			m_documentType->itemData(m_documentType->currentIndex()).toInt();
	m_document.serialNumber = m_serialNumber->text();
	m_document.number = m_number->text();
	m_document.date = m_date->date();
	m_document.givenBy = m_givenBy->text();

	return m_document;
}
