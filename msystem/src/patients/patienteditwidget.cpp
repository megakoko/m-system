#include "patienteditwidget.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include "macros.h"

#include "addressdialog.h"
#include "documenteditdialog.h"


namespace DocumentTableColumns
{
	enum {
		DocumentName = 0,
		Date,
		Number,
		Count
	};
}


PatientEditWidget::PatientEditWidget(const int patientId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_patientId(patientId)
	, m_mailingAddress(Address(true))
	, m_actualAddress(Address(false))
{
	setupUi(this);

	init();
	initConnections();
}


void PatientEditWidget::init()
{
	m_editDocument->setEnabled(false);
	m_deleteDocument->setEnabled(false);

	m_documentTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_documentTable->setColumnCount(DocumentTableColumns::Count);
	m_documentTable->setHorizontalHeaderLabels(QStringList()
					<< QString::fromUtf8("Документ")
					<< QString::fromUtf8("Дата выдачи")
					<< QString::fromUtf8("Серия, номер"));


	QSqlQuery q;
	q.prepare(" SELECT familyName, name, patronymic, birthDay "
			  " FROM Patient "
			  " WHERE id = :patientId");
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);

	const bool idIsValid = q.first();
	Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

	m_familyName->setText(q.value(0).toString());
	m_name->setText(q.value(1).toString());
	m_patronymic->setText(q.value(2).toString());
	m_birthDay->setDate(q.value(3).toDate());

	q.prepare(" SELECT id, isMailingAddress, city, street, house, apartment "
			  " FROM Address "
			  " WHERE patientId = :patientId "
			  " ORDER BY isMailingAddress DESC "); // Сначала TRUE, потом FALSE
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);


	m_mailingAddressIsActual->setChecked(true);
	toggleMailingAddressIsActual(true);
	if(q.first())
	{
		m_mailingAddress = Address(q.record());
		m_mailingAddressLabel->setText(m_mailingAddress.toString());

		// Адрес по прописке должен существовать.
		Q_ASSERT(q.record().value("isMailingAddress").toBool() == true);


		if(q.next())
		{
			m_mailingAddressIsActual->setChecked(false);
			toggleMailingAddressIsActual(false);

			m_actualAddress = Address(q.record());
			m_actualAddressLabel->setText(m_actualAddress.toString());
		}
	}



	q.prepare(" SELECT id, documentTypeId, serialNumber, date, givenBy "
			  " FROM Document "
			  " WHERE patientId = :patientId "
			  " ORDER BY documentTypeId, date ");
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);

	while(q.next())
	{
		m_documents << Document(q.record());
		addDocumentToTable(m_documents.last());
	}

}


void PatientEditWidget::initConnections()
{
	connect(m_familyName, SIGNAL(editingFinished()), SLOT(patientNameEdited()));
	connect(m_name, SIGNAL(editingFinished()), SLOT(patientNameEdited()));
	connect(m_patronymic, SIGNAL(editingFinished()), SLOT(patientNameEdited()));


	connect(m_mailingAddressIsActual,
			SIGNAL(toggled(bool)),
			SLOT(toggleMailingAddressIsActual(bool)));

	connect(m_editMailingAddress, SIGNAL(clicked()), SLOT(launchMailingAddressEditing()));
	connect(m_editActualAddress, SIGNAL(clicked()), SLOT(launchActualAddressEditing()));

	connect(m_save, SIGNAL(clicked()), SIGNAL(closeMe()));

	connect(m_documentTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(documentSelectionChanged()));
	connect(m_addDocument, SIGNAL(clicked()), SLOT(addDocument()));
	connect(m_editDocument, SIGNAL(clicked()), SLOT(editDocument()));
	connect(m_deleteDocument, SIGNAL(clicked()), SLOT(deleteDocument()));
}


void PatientEditWidget::toggleMailingAddressIsActual(bool checked)
{
	m_actualAddressLabel->setEnabled(!checked);
	m_editActualAddress->setEnabled(!checked);
}


void PatientEditWidget::launchMailingAddressEditing()
{
	AddressDialog d(m_mailingAddress, this);
	if(d.exec() == QDialog::Accepted)
	{
		m_mailingAddress = d.address();
		m_mailingAddressLabel->setText(m_mailingAddress.toString());
	}
}


void PatientEditWidget::launchActualAddressEditing()
{
	AddressDialog d(m_actualAddress, this);
	if(d.exec() == QDialog::Accepted)
	{
		m_actualAddress = d.address();
		m_actualAddressLabel->setText(m_actualAddress.toString());
	}
}


int PatientEditWidget::selectedDocumentsCount() const
{
	int result = 0;
	if(m_documentTable->selectionModel()->hasSelection())
		result = m_documentTable->selectionModel()->selectedRows().count();

	return result;
}


void PatientEditWidget::documentSelectionChanged()
{
	const int count = selectedDocumentsCount();

	m_editDocument->setEnabled(count == 1);
	m_deleteDocument->setEnabled(count == 1);
}


void PatientEditWidget::addDocument()
{
	DocumentEditDialog d(Document(), this);
	if(d.exec() == QDialog::Accepted)
	{
		m_documents.append(d.document());
		addDocumentToTable(m_documents.last());
	}


}


void PatientEditWidget::editDocument()
{
	const int tableRow = m_documentTable->currentRow();

	Q_ASSERT(selectedDocumentsCount() == 1);
	Q_ASSERT(tableRow < m_documents.count());

	DocumentEditDialog d(m_documents[tableRow], this);
	if(d.exec() == QDialog::Accepted)
	{
		m_documents[tableRow] = d.document();
		editDocumentInTable(tableRow, m_documents[tableRow]);
	}
}


void PatientEditWidget::deleteDocument()
{
	const int tableRow = m_documentTable->currentRow();

	Q_ASSERT(selectedDocumentsCount() == 1);
	Q_ASSERT(tableRow < m_documents.count());


	const QString& title = QString::fromUtf8("Удаление документа");
	const QString& descr = QString::fromUtf8(
				"Вы действительно хотите удалить документ?");


	const int rc =
		QMessageBox::question(this, title, descr, QMessageBox::Yes | QMessageBox::No);
	if(rc == QMessageBox::Yes)
	{
		m_documentsMarkedForDeletion.append(m_documents.takeAt(tableRow));
		deleteDocumentFromTable(tableRow);
	}
}


void PatientEditWidget::addDocumentToTable(const Document& doc)
{
	const int row = m_documentTable->rowCount();
	m_documentTable->insertRow(row);

	QTableWidgetItem* item;

	item = new QTableWidgetItem(doc.documentName());
	m_documentTable->setItem(row, DocumentTableColumns::DocumentName, item);

	item = new QTableWidgetItem(doc.date.toString());
	m_documentTable->setItem(row, DocumentTableColumns::Date, item);

	item = new QTableWidgetItem(doc.serialNumber);
	m_documentTable->setItem(row, DocumentTableColumns::Number, item);
}


void PatientEditWidget::editDocumentInTable(const int row, const Document& newDoc)
{
	QTableWidgetItem* item;

	item = m_documentTable->item(row, DocumentTableColumns::DocumentName);
	item->setText(newDoc.documentName());

	item = m_documentTable->item(row, DocumentTableColumns::Date);
	item->setText(newDoc.date.toString());

	item = m_documentTable->item(row, DocumentTableColumns::Number);
	item->setText(newDoc.serialNumber);
}


void PatientEditWidget::deleteDocumentFromTable(const int row)
{
	m_documentTable->removeRow(row);
}


bool PatientEditWidget::canSave(QString &errorDescription) const
{
	if (m_familyName->text().simplified().isEmpty() ||
		m_name->text().simplified().isEmpty() ||
		m_patronymic->text().simplified().isEmpty())
	{
		errorDescription = QString::fromUtf8("Имя пациента не заполнено");
		return false;
	}
	else if(m_mailingAddress.isNull())
	{
		errorDescription = QString::fromUtf8("Адрес прописки не заполнен");
		return false;
	}

	return true;
}


void PatientEditWidget::save()
{
	QSqlQuery q;
	q.prepare(" UPDATE Patient SET "
			  " familyName = :familyName, "
			  " name = :name, "
			  " patronymic = :patronymic, "
			  " birthDay = :birthDay "
			  " WHERE id = :patientId ");
	q.bindValue(":familyName", m_familyName->text());
	q.bindValue(":name", m_name->text());
	q.bindValue(":patronymic", m_patronymic->text());
	q.bindValue(":birthDay", m_birthDay->date());
	q.bindValue(":patientId", m_patientId);
	q.exec();
	checkQuery(q);


	m_mailingAddress.save(m_patientId);
	if(m_mailingAddressIsActual->isChecked())
		m_actualAddress.deleteAddress();
	else
		m_actualAddress.save(m_patientId);


	foreach(const Document& doc, m_documents)
		doc.save(m_patientId);
	foreach(const Document& doc, m_documentsMarkedForDeletion)
		doc.deleteDocument();

	emit saved();
}


void PatientEditWidget::patientNameEdited()
{
	emit setTabLabel(fullPatientName());
}


QString PatientEditWidget::fullPatientName() const
{
	return QString::fromUtf8("Пациент %1 %2.%3.").
				arg(m_familyName->text()).
				arg(m_name->text().left(1)).
				arg(m_patronymic->text().left(1));
}
