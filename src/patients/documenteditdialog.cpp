#include "documenteditdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include "macros.h"
#include "patients.h"



DocumentEditDialog::DocumentEditDialog(const Document& doc, QWidget *parent)
	: QDialog(parent)
	, m_document(doc)
{
    setupUi(this);


	m_serialNumber->setMaxLength(Patients::interfaces->db->
								 fieldMaximumLength("Document", "serialnumber"));

	m_givenBy->setMaxLength(Patients::interfaces->db->
							fieldMaximumLength("Document", "givenBy"));


	QSqlQuery q("SELECT name, id FROM DocumentType ORDER BY id");
	while(q.next())
		m_documentType->addItem(q.value(0).toString(), q.value(1));
	m_documentType->setCurrentIndex(m_documentType->findData(doc.documentTypeId));

	if(doc.givenBy.isNull() && doc.serialNumber.isNull() && doc.date.isNull())
		setWindowTitle("Добавление документа");
	else
	{
		m_serialNumber->setText(doc.serialNumber);
		m_date->setDate(doc.date);
		m_givenBy->setText(doc.givenBy);
	}
	checkFields();


	resize(width(), 0);


	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_documentType, SIGNAL(currentIndexChanged(int)), SLOT(checkFields()));
	connect(m_serialNumber, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_givenBy, SIGNAL(textChanged(QString)), SLOT(checkFields()));
}


Document DocumentEditDialog::document()
{
	m_document.documentTypeId =
			m_documentType->itemData(m_documentType->currentIndex()).toInt();
	m_document.serialNumber = nullIfEmpty(m_serialNumber->text().simplified());
	m_document.date = m_date->date();
	m_document.givenBy = nullIfEmpty(m_givenBy->text().simplified());

	return m_document;
}


void DocumentEditDialog::checkFields()
{
	const bool fieldsAreValid =
			m_documentType->currentIndex() >= 0 &&
			!m_serialNumber->text().simplified().isEmpty() &&
			!m_givenBy->text().simplified().isEmpty();

	m_ok->setEnabled(fieldsAreValid);
}
