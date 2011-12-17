#pragma once

#include "ui_addressdialog.h"
#include "address.h"


class AddressDialog : public QDialog, private Ui::AddressDialog
{
	Q_OBJECT

public:
	explicit AddressDialog(const Address& address, QWidget *parent = 0);
	Address address();

private:
	Address m_address;

private slots:
	void checkFields();

};
