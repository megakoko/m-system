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


	if(m_staffPosition.m_staffId <= 0 && m_staffPosition.m_positionId <= 0)
		setWindowTitle("Добавление должности сотрудника");


	QSqlQuery q(" SELECT id, familyName || ' ' || name || ' ' || patronymic "
				" FROM Staff ORDER BY familyName, name, patronymic");
	checkQuery(q);

	while(q.next())
		m_staff->addItem(q.value(1).toString(), q.value(0));
	m_staff->setCurrentIndex(m_staff->findData(m_staffPosition.m_staffId));


	q.exec("SELECT id, name FROM Position ORDER BY name");
	checkQuery(q);

	while(q.next())
		m_position->addItem(q.value(1).toString(), q.value(0));
	m_position->setCurrentIndex(m_position->findData(m_staffPosition.m_positionId));


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
