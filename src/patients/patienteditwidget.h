#pragma once


#include "../loader/pluginwidget.h"
#include "ui_patienteditwidget.h"

#include "address.h"

class PatientEditWidget : public PluginWidget, private Ui::PatientEditWidget
{
	Q_OBJECT

public:
	explicit PatientEditWidget(const int patientId, QWidget *parent = 0);

private:
	void init();
	void initConnections();


	const int m_patientId;

	Address m_mailingAddress;
	Address m_actualAddress;

private slots:
	void toggleMailingAddressIsActual(bool checked);

	void launchMailingAddressEditing();
	void launchActualAddressEditing();

	void save();
};
