#pragma once


#include "saveablepluginwidget.h"
#include "ui_patienteditwidget.h"

#include "address.h"
#include "document.h"

class PatientEditWidget : public SaveablePluginWidget, private Ui::PatientEditWidget
{
	Q_OBJECT

public:
	explicit PatientEditWidget(const int patientId, QWidget *parent = 0);

	bool canSave(QString &errorDescription) const;
	void save();

	QString fullPatientName() const;

private:
	void init();
	void initConnections();

	int selectedDocumentsCount() const;

	void addDocumentToTable(const Document& doc);
	void editDocumentInTable(const int row, const Document& newDoc);
	void deleteDocumentFromTable(const int row);

	int m_patientId;

	Address m_mailingAddress;
	Address m_actualAddress;

	QList<Document> m_documents;
	QList<Document> m_documentsMarkedForDeletion;

private slots:
	void toggleMailingAddressIsActual(bool checked);

	void launchMailingAddressEditing();
	void launchActualAddressEditing();

	void documentSelectionChanged();
	void addDocument();
	void editDocument();
	void deleteDocument();

	void patientNameEdited();

};
