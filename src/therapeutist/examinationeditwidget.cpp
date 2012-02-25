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
#include "therapeutistpickerdialog.h"



ExaminationEditWidget::ExaminationEditWidget(const int examinationId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_examinationId(examinationId)
	, m_patientId(InvalidId)
	, m_therapeutistId(InvalidId)
{
	setupUi(this);
	init();
	initConnections();
}


void ExaminationEditWidget::init()
{
	m_mainContainer = new ExamContainer(m_examinationId, "main", true);
	m_widgetsLayout->addWidget(m_mainContainer->widget(), 0, Qt::AlignTop);
	m_mainContainer->setParent(this);


	// Если текущий пользователь не администратор, то отключаем возможность
	// выбора доктора, проводившего осмотр.
	if(!Therapeutist::interfaces->usr->currentUserIsAdmin())
		m_chooseTherapeutist->setEnabled(false);


	if(m_examinationId == InvalidId)
	{
		m_examDate->setDateTime(QDateTime::currentDateTime());


		// Если текущий пользователь (не являющийся админом) создает осмотр, автоматически
		// определяем ID врача и его имя.
		if(!Therapeutist::interfaces->usr->currentUserIsAdmin())
		{
			QSqlQuery q;
			q.prepare(" SELECT s.id, s.familyname || ' ' || s.name || ' ' || s.patronymic "
					  " FROM MUser u "
					  " LEFT JOIN Staff s ON u.attachedStaffId = s.id "
					  " LEFT JOIN DepartmentStaffPosition dsp ON s.id = dsp.staffId "
					  " LEFT JOIN Position p ON dsp.positionId = p.id "
					  " WHERE u.id = ? AND p.textid = ?");

			q.addBindValue(Therapeutist::interfaces->usr->currentUserId());
			q.addBindValue("therapeutist");	// На всякий случай пусть отсеются все не-терапевты.
			q.exec();
			checkQuery(q);

			if(q.first())
			{
				m_therapeutistId = q.value(0).toInt();
				m_therapeutistName->setText(q.value(1).toString());
			}
		}
	}
	else
	{
		QSqlQuery q;
		q.prepare(" SELECT patientId, examinationDate, examinedByStaffId, "
				  " s.familyName || ' ' || s.name || ' ' || s.patronymic "
				  " FROM Examination e "
				  " LEFT JOIN Staff s ON e.examinedByStaffId = s.id "
				  " WHERE e.id = ? ");
		q.addBindValue(m_examinationId);
		q.exec();
		checkQuery(q);

		const int idIsValid = q.first();
		Q_ASSERT(idIsValid); Q_UNUSED(idIsValid);

		m_patientId = q.value(0).toInt();
		m_patientName->setText(patientName(m_patientId).join(" "));
		m_examDate->setDateTime(q.value(1).toDateTime());
		m_therapeutistId = q.value(2).toInt();
		m_therapeutistName->setText(q.value(3).toString());
	}
}


void ExaminationEditWidget::initConnections()
{
	connect(m_choosePatient, SIGNAL(clicked()), SLOT(choosePatient()));
	connect(m_chooseTherapeutist, SIGNAL(clicked()), SLOT(chooseTherapeutist()));
}


QString ExaminationEditWidget::tabName(const int patientId)
{
	QString result = "Осмотр пациента";

	if(patientId != InvalidId)
	{
		const QStringList& patientNameList = patientName(patientId);

		const QString& familyName = patientNameList[0];
		const QString& firstName = patientNameList[1];
		const QString& patronymic = patientNameList[2];


		result += ": " + familyName;

		if(!firstName.isEmpty())
		{
			result += " ";
			result += firstName[0];
			result += ".";
		}

		if(!patronymic.isEmpty())
		{
			result += " ";
			result += patronymic[0];
			result += ".";
		}
	}

	return result;
}


bool ExaminationEditWidget::canSave(QString &errorDescription) const
{
	if(m_patientId == InvalidId)
	{
		errorDescription = "Не выбран пациент";
		return false;
	}
	else if(m_therapeutistId == InvalidId)
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
	q.bindValue(":examinedByStaffId", m_therapeutistId);

	q.exec();
	checkQuery(q);

	if(m_examinationId == InvalidId)
		m_examinationId = Therapeutist::interfaces->db->lastInsertedId(&q).toInt();

	// Сохраняем введенные симптомы.
	m_mainContainer->save(m_examinationId);
	// Сохраняем возраст.
	savePatientAge();

	q.exec("COMMIT");
	emit saved();
}


int yearsBetween(const QDate& date1, const QDate& date2)
{
	int years = -1;

	if(date2 > date1)
	{
		years = date2.year() - date1.year();

		if(date2.month() < date1.month() ||
		   (date2.month() == date1.month() && date2.day() <= date1.day()))
		{
			years = years - 1;
		}
	}

	return years;
}


void ExaminationEditWidget::savePatientAge()
{
	// Сначала узнаем день рождения пациента.
	QSqlQuery q;
	q.prepare(" SELECT birthday FROM Patient "
			  " WHERE id = (SELECT patientId FROM Examination WHERE id = ?) ");
	q.addBindValue(m_examinationId);
	q.exec();
	checkQuery(q);

	if(q.first())
	{
		const QDate& birthday = Therapeutist::interfaces->enc->decodeDate(q.value(0).toString());
		const QDate& examDate = m_examDate->date();

		const int age = yearsBetween(birthday, examDate);
		if(age < 0)
			return;


		q.prepare(" SELECT id "
				  " FROM ExaminationData "
				  " WHERE uiElementId = (SELECT id FROM UiElement WHERE textid = ?)");
		q.addBindValue("age");
		q.exec();
		checkQuery(q);

		QVariant ageId;
		if(q.first())
			ageId = q.value(0);


		if(ageId.isValid())
		{
			q.prepare(" UPDATE ExaminationData SET "
					  " realValue = :age "
					  " WHERE id = :id AND examinationId = :examId ");
			q.bindValue(":id", ageId);
		}
		else
		{
			q.prepare(" INSERT INTO ExaminationData "
					  " ( examinationId,  uiElementId,  realValue) VALUES "
					  " (:examId, (SELECT id FROM UiElement WHERE textid = 'age'), :age)");
		}
		q.bindValue(":age", age);
		q.bindValue(":examId", m_examinationId);
		q.exec();
		checkQuery(q);
	}
}


QStringList ExaminationEditWidget::patientName(const int patientId)
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

	return patientName;
}


void ExaminationEditWidget::choosePatient()
{
	PatientPickerDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_patientId = d.selectedPatientId();


		m_patientName->setText(patientName(m_patientId).join(" "));
		emit requestToSetTabLabel(tabName(m_patientId));
	}
}


void ExaminationEditWidget::chooseTherapeutist()
{
	TherapeutistPickerDialog d(this);
	if(d.exec() == QDialog::Accepted)
	{
		m_therapeutistId = d.selectedTherapeutistId();
		m_therapeutistName->setText(d.selectedTherapeutistName());
	}
}
