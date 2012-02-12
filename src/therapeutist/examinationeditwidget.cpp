#include "examinationeditwidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QDateTime>
#include <QDebug>

#include "macros.h"
#include "therapeutist.h"
#include "examcontainer.h"
#include "patientpickerdialog.h"



ExaminationEditWidget::ExaminationEditWidget(const int examinationId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_examinationId(examinationId)
	, m_patientId(InvalidId)
{
	setupUi(this);
	init();
	initConnections();
}


void ExaminationEditWidget::init()
{
	m_mainContainer = new ExamContainer(m_examinationId, "main", true);
	m_widgetsLayout->addWidget(m_mainContainer->widget(), 0, Qt::AlignTop);


	QSqlQuery q;

	q.prepare(" SELECT DISTINCT s.id, p.name, s.familyName, s.name, s.patronymic  "
			  " FROM DepartmentStaffPosition dsp "
			  " LEFT JOIN Staff s ON dsp.staffId = s.id "
			  " LEFT JOIN Position p ON dsp.positionId = p.id "
			  " WHERE p.textid = ? "
			  " ORDER BY s.familyName, s.name, s.patronymic ");
	q.addBindValue("therapeutist");
	q.exec();
	checkQuery(q);
	while(q.next())
	{
		const QString& position = q.value(1).toString();
		const QString& name = q.value(2).toString() + " " +
							  q.value(3).toString() + " " +
							  q.value(4).toString();

		m_staff->addItem(position + " " + name, q.value(0));
	}
	m_staff->setCurrentIndex(-1);



	if(m_examinationId == InvalidId)
	{
		m_examDate->setDateTime(QDateTime::currentDateTime());
	}
	else
	{
		q.prepare(" SELECT patientId, examinationDate, examinedByStaffId "
				  " FROM Examination WHERE id = ? ");
		q.addBindValue(m_examinationId);
		q.exec();
		checkQuery(q);

		const int idIsValid = q.first();
		Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

		m_patientId = q.value(0).toInt();
		m_patientName->setText(patientName(m_patientId));
		m_examDate->setDateTime(q.value(1).toDateTime());
		m_staff->setCurrentIndex(m_staff->findData(q.value(2)));
	}
}


void ExaminationEditWidget::initConnections()
{
	connect(m_choosePatient, SIGNAL(clicked()), SLOT(choosePatient()));
	connect(m_save, SIGNAL(clicked()), SIGNAL(requestForSaving()));
}


bool ExaminationEditWidget::canSave(QString &errorDescription) const
{
	if(m_patientId == InvalidId)
	{
		errorDescription = "Не выбран пациент";
		return false;
	}
	else if(m_staff->currentIndex() == -1)
	{
		errorDescription = "Не выбран врач, проводящий осмотр";
		return false;
	}

	return true;
}


void ExaminationEditWidget::save()
{
	QSqlQuery q;
	q.exec("BEGIN");

	if(m_examinationId == InvalidId)
	{
		q.prepare(" INSERT INTO Examination "
				  " ( patientId,  examinationDate,  examinedByStaffId) VALUES "
				  " (:patientId, :examinationDate, :examinedByStaffId) " +
				  Therapeutist::interfaces->db->returningSentence("id"));

	}
	else
	{
		q.prepare(" UPDATE Examination SET "
				  " patientId = :patientId, "
				  " examinationdate = :examinationDate, "
				  " examinedByStaffId = :examinedByStaffId "
				  " WHERE id = :id ");
		q.bindValue(":id", m_examinationId);
	}
	q.bindValue(":patientId", m_patientId);
	q.bindValue(":examinationDate", m_examDate->dateTime());
	q.bindValue(":examinedByStaffId", m_staff->itemData(m_staff->currentIndex()));

	q.exec();
	checkQuery(q);

	if(m_examinationId == InvalidId)
		m_examinationId = Therapeutist::interfaces->db->lastInsertedId(&q).toInt();


	m_mainContainer->save(m_examinationId);

	q.exec("COMMIT");
	emit saved();
}


QString ExaminationEditWidget::patientName(const int patientId)
{
	QSqlQuery q;
	q.prepare("SELECT familyname, name, patronymic FROM Patient WHERE id = ?");
	q.addBindValue(patientId);
	q.exec();
	checkQuery(q);

	QStringList patientName;
	if(q.first())
		for(int i = 0; i <= 2; ++i)
			patientName += Therapeutist::interfaces->enc->decodeStr(q.value(i).toString());

	return patientName.join(" ");
}


void ExaminationEditWidget::choosePatient()
{
	PatientPickerDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_patientId = d.selectedPatientId();

		m_patientName->setText(patientName(m_patientId));
	}
}
