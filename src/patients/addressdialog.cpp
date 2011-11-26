#include "addressdialog.h"
#include "macros.h"


AddressDialog::AddressDialog(const Address& address, QWidget *parent)
	: QDialog(parent)
	, m_address(address)
{
	setupUi(this);

	m_city->setText(m_address.city);
	m_street->setText(m_address.street);
	m_house->setText(m_address.house);
	m_apartment->setText(m_address.apartment);
	checkFields();


	resize(width(), 0);


	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_city, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_street, SIGNAL(textChanged(QString)), SLOT(checkFields()));
	connect(m_house, SIGNAL(textChanged(QString)), SLOT(checkFields()));
}


Address AddressDialog::address()
{
	m_address.city		= nullIfEmpty(m_city->text().simplified());
	m_address.street	= nullIfEmpty(m_street->text().simplified());
	m_address.house		= nullIfEmpty(m_house->text().simplified());
	m_address.apartment	= nullIfEmpty(m_apartment->text().simplified());

	return m_address;
}


void AddressDialog::checkFields()
{
	const bool fieldsAreValid =
			!m_city->text().simplified().isEmpty() &&
			!m_street->text().simplified().isEmpty() &&
			!m_house->text().simplified().isEmpty();


	m_ok->setEnabled(fieldsAreValid);
}
