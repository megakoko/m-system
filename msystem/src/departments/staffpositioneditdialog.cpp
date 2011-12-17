#include "staffpositioneditdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


#include "macros.h"


StaffPositionEditDialog::StaffPositionEditDialog(const StaffPosition& sp, QWidget *parent)
	: QDialog(parent)
	, m_staffPosition(sp)
{
    setupUi(this);
	init();
}


void StaffPositionEditDialog::init()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));
	connect(m_staff, SIGNAL(currentIndexChanged(int)), SLOT(checkCombos()));
	connect(m_position, SIGNAL(currentIndexChanged(int)), SLOT(checkCombos()));



	QSqlQuery q(" SELECT id, familyName || ' ' || name || ' ' || patronymic "
				" FROM Staff ORDER BY familyName, name, patronymic");
	checkQuery(q);


	m_staff->setCurrentIndex(-1);
	while(q.next())
	{
		const QVariant& id = q.value(0);
		m_staff->addItem(q.value(1).toString(), id);

		if(!id.isNull() && id == m_staffPosition.m_staffId)
			m_staff->setCurrentIndex(m_staff->count() - 1);
	}

	q.exec("SELECT id, name FROM Position ORDER BY name");
	checkQuery(q);

	m_position->setCurrentIndex(-1);
	while(q.next())
	{
		const QVariant& id = q.value(0);
		m_position->addItem(q.value(1).toString(), id);

		if(!id.isNull() && id == m_staffPosition.m_positionId)
			m_position->setCurrentIndex(m_position->count() - 1);
	}


	checkCombos();
}



QVariant StaffPositionEditDialog::currentStaffId() const
{
	return m_staff->itemData(m_staff->currentIndex());
}


QVariant StaffPositionEditDialog::currentPositionId() const
{
	return m_position->itemData(m_position->currentIndex());
}


void StaffPositionEditDialog::checkCombos()
{
	if(m_position->currentIndex() > -1 && m_staff->currentIndex() > -1)
	{
		m_ok->setEnabled(true);
		disconnect(m_position, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCombos()));
		disconnect(m_staff, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCombos()));
	}
}


StaffPosition StaffPositionEditDialog::currentStaffPosition()
{
	const QVariant& staffId = currentStaffId();
	const QVariant& positionId = currentPositionId();

	Q_ASSERT(!staffId.isNull() && !positionId.isNull());

	m_staffPosition.m_staffId = staffId.toInt();
	m_staffPosition.m_positionId = positionId.toInt();

	return m_staffPosition;
}
