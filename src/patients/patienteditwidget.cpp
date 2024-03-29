#include "patienteditwidget.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include "macros.h"

#include "patients.h"
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


PatientEditWidget::PatientEditWidget(const QString &familyName,
									 const QString &name,
									 const QString &patronymic,
									 QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_patientId(InvalidId)
	, m_mailingAddress(Address("mailing"))
	, m_actualAddress(Address("actual"))
{
	setupUi(this);

	init();
	initConnections();

	m_familyName->setText(familyName);
	m_name->setText(name);
	m_patronymic->setText(patronymic);
}


PatientEditWidget::PatientEditWidget(const int patientId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_patientId(patientId)
	, m_mailingAddress(Address("mailing"))
	, m_actualAddress(Address("actual"))
{
	setupUi(this);

	init();
	initConnections();
}


void PatientEditWidget::init()
{
	m_familyName->setMaxLength(Patients::interfaces->db->
							   fieldMaximumLength("Patient", "familyName"));

	m_name->setMaxLength(Patients::interfaces->db->
						 fieldMaximumLength("Patient", "name"));

	m_patronymic->setMaxLength(Patients::interfaces->db->
							   fieldMaximumLength("Patient", "patronymic"));


	m_editDocument->setEnabled(false);
	m_deleteDocument->setEnabled(false);

	m_documentTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_documentTable->setColumnCount(DocumentTableColumns::Count);
	m_documentTable->setHorizontalHeaderLabels(QStringList()
					<< QString::fromUtf8("Документ")
					<< QString::fromUtf8("Дата выдачи")
					<< QString::fromUtf8("Серия, номер"));


	if(m_patientId != InvalidId)
	{
		QSqlQuery q;
		q.prepare(" SELECT familyName, p.name, patronymic, birthDay, s.textid "
				  " FROM Patient p LEFT JOIN Sex s ON p.sexId = s.id "
				  " WHERE p.id = :patientId");
		q.bindValue(":patientId", m_patientId);
		q.exec();
		checkQuery(q);

		const bool idIsValid = q.first();
		Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

		m_familyName->setText(Patients::interfaces->enc->decodeStr(q.value(0).toString()));
		m_name->setText(Patients::interfaces->enc->decodeStr(q.value(1).toString()));
		m_patronymic->setText(Patients::interfaces->enc->decodeStr(q.value(2).toString()));
		m_birthDay->setDate(Patients::interfaces->enc->decodeDate(q.value(3).toString()));

		if(q.value(4).toString() == "male")
			m_male->setChecked(true);
		else if(q.value(4).toString() == "female")
			m_female->setChecked(true);
		else
			Q_ASSERT(!"Unknown option");


		q.prepare(" SELECT a.id, at.textid, region, city, street, house, apartment "
				  " FROM Address a "
				  " LEFT JOIN AddressType at ON a.typeId = at.id"
				  " WHERE patientId = :patientId ");
		q.bindValue(":patientId", m_patientId);
		q.exec();
		checkQuery(q);


		bool hasMailingAddress = false;
		bool hasActualAddress = false;
		while(q.next())
		{
			const QSqlRecord& rec = q.record();
			if(rec.value("textid").toString() == "mailing")
			{
				m_mailingAddress = Address(rec);
				hasMailingAddress = true;
			}
			else if(rec.value("textid").toString() == "actual")
			{
				m_actualAddress = Address(rec);
				hasActualAddress = true;
			}
			else
				qWarning() << "Unknown address type at" << __FILE__ << __LINE__;
		}

		// Должен существовать хотя бы адрес по прописке.
		Q_ASSERT(hasMailingAddress); Q_UNUSED(hasMailingAddress);
		m_mailingAddressIsActual->setChecked(!hasActualAddress);
		toggleMailingAddressIsActual(!hasActualAddress);


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

	// Адреса.
	m_mailingAddressLabel->setText(m_mailingAddress.toString());
	m_actualAddressLabel->setText(m_actualAddress.toString());
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

	connect(m_documentTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(editDocument()));
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

	item = new QTableWidgetItem(doc.date.toString("dd.MM.yyyy"));
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
	item->setText(newDoc.date.toString("dd.MM.yyyy"));

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
	}else if(m_mailingAddress.isNull())
	{
		errorDescription = QString::fromUtf8("Адрес прописки не заполнен");
		return false;
	}
	else if(!m_mailingAddressIsActual->isChecked() && m_actualAddress.isNull())
	{
		errorDescription = "Адрес проживания не заполнен";
		return false;
	}
	else if(!m_male->isChecked() && !m_female->isChecked())
	{
		errorDescription = "Пол пациента не выбран";
		return false;
	}

	return true;
}


void PatientEditWidget::save()
{
	QSqlQuery q;

	if(m_patientId == InvalidId)
	{
		q.prepare(" INSERT INTO Patient "
				  " ( familyName,  name,  patronymic,  birthDay,  sexid) VALUES "
				  " (:familyName, :name, :patronymic, :birthDay, "
						" (SELECT id FROM Sex WHERE textid = :settextid)) " +
				  Patients::interfaces->db->returningSentence("id"));
	}
	else
	{
		q.prepare(" UPDATE Patient SET "
				  " familyName = :familyName, "
				  " name = :name, "
				  " patronymic = :patronymic, "
				  " birthDay = :birthDay, "
				  " sexid = (SELECT id FROM Sex WHERE textid = :sextextid) "
				  " WHERE id = :patientId ");
		q.bindValue(":patientId", m_patientId);
	}
	q.bindValue(":familyName", Patients::interfaces->enc->encodeStr(m_familyName->text()));
	q.bindValue(":name", Patients::interfaces->enc->encodeStr(m_name->text()));
	q.bindValue(":patronymic", Patients::interfaces->enc->encodeStr(m_patronymic->text()));
	q.bindValue(":birthDay", Patients::interfaces->enc->encodeDate(m_birthDay->date()));

	if(m_male->isChecked())
		q.bindValue(":sextextid", "male");
	else if(m_female->isChecked())
		q.bindValue(":sextextid", "female");
	else
		Q_ASSERT(!"Unknown option");


	q.exec();
	checkQuery(q);

	if(m_patientId == InvalidId)
		m_patientId = Patients::interfaces->db->lastInsertedId(&q).toInt();


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
	emit requestToSetTabLabel(fullPatientName());
}


QString PatientEditWidget::fullPatientName() const
{
	return QString::fromUtf8("Пациент %1 %2.%3.").
				arg(m_familyName->text()).
				arg(m_name->text().left(1)).
				arg(m_patronymic->text().left(1));
}
