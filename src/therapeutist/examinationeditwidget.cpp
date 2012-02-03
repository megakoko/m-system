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

	Q_ASSERT(InvalidId == ExamWidget::InvalidId);
	Q_ASSERT(InvalidId == PatientPickerDialog::InvalidId);
}


void ExaminationEditWidget::init()
{
	m_mainContainer = new ExamContainer(ExamWidget::InvalidId, "main", QString::null);
	m_widgetsLayout->addWidget(m_mainContainer);

	m_examDate->setDateTime(QDateTime::currentDateTime());

	if(m_examinationId != InvalidId)
	{
		// TODO.
	}
}


void ExaminationEditWidget::initConnections()
{
	connect(m_choosePatient, SIGNAL(clicked()), SLOT(choosePatient()));
}


bool ExaminationEditWidget::canSave(QString &errorDescription) const
{
	if(m_patientId == InvalidId)
	{
		errorDescription = "Не выбран пациент";
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
				  " ( patientId,  examinationDate) VALUES "
				  " (:patientId, :examinationDate) " +
				  Therapeutist::interfaces->db->returningSentence("id"));

	}
	else
	{
		q.prepare(" UPDATE Examination SET "
				  " patientId = :patientId, "
				  " examinatinodate = :examinationDate "
				  " WHERE id = :id ");
		q.bindValue(":id", m_examinationId);
	}
	q.bindValue(":patientId", m_patientId);
	q.bindValue(":examinationDate", m_examDate->dateTime());

	q.exec();
	checkQuery(q);

	if(m_examinationId == InvalidId)
		m_examinationId = Therapeutist::interfaces->db->lastInsertedId(&q).toInt();


	m_mainContainer->save(m_examinationId);

	q.exec("COMMIT");
	emit saved();
}


void ExaminationEditWidget::choosePatient()
{
	PatientPickerDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_patientId = d.selectedPatientId();

		QSqlQuery q;
		q.prepare("SELECT familyname, name, patronymic FROM Patient WHERE id = ?");
		q.addBindValue(m_patientId);
		q.exec();
		checkQuery(q);

		if(q.first())
		{
			QStringList patientName;

			for(int i = 0; i <= 2; ++i)
				patientName += Therapeutist::interfaces->enc->decode(q.value(i).toString());

			m_patientName->setText(patientName.join(" "));
		}
	}
}
