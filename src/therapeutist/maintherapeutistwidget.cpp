#include "maintherapeutistwidget.h"
#include "examinationeditwidget.h"

#include <QMessageBox>

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "components/decodingproxymodel.h"
#include "components/columnjoiningproxymodel.h"

#include "therapeutist.h"
#include "macros.h"
#include "examinationpreview.h"
#include "examinationlistreport.h"


MainTherapeutistWidget::MainTherapeutistWidget(QWidget* parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainTherapeutistWidget::init()
{
	m_queryModel = new QSqlQueryModel(this);

	DecodingProxyModel* m_proxyModel = new DecodingProxyModel(this);
	m_proxyModel->setInterfacesPtr(Therapeutist::interfaces);
	m_proxyModel->addEncodedStringColumn(2);
	m_proxyModel->addEncodedStringColumn(3);
	m_proxyModel->addEncodedStringColumn(4);
	m_proxyModel->setModel(m_queryModel);

	ColumnJoiningProxyModel* columnJoining = new ColumnJoiningProxyModel(this);
	columnJoining->setJoinedColumns(2, 3, "Имя пациента");
	columnJoining->setModel(m_proxyModel);

	updateExaminationList();

	m_examinations->setModel(columnJoining);
	m_examinations->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
	m_examinations->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
	m_examinations->setColumnHidden(0, true);

	examSelectionChanged();
}


void MainTherapeutistWidget::initConnections()
{
	connect(m_examinations->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(examSelectionChanged()));

	connect(m_examinations, SIGNAL(doubleClicked(QModelIndex)), SLOT(editExam()));

	connect(m_addExam, SIGNAL(clicked()), SLOT(addExam()));
	connect(m_editExam, SIGNAL(clicked()), SLOT(editExam()));
	connect(m_deleteExam, SIGNAL(clicked()), SLOT(deleteExam()));

	connect(m_openPreview, SIGNAL(clicked()), SLOT(openExamPreview()));
	connect(m_openExamListReport, SIGNAL(clicked()), SLOT(openExamListReport()));
}


QString MainTherapeutistWidget::examinationListQuery()
{
	static const QString select=" SELECT e.id, %1 AS \"Время осмотра\", "
								" p.familyName, p.name, p.patronymic ";
	static const QString psqlSelect = select.arg("to_char(e.examinationDate, 'dd.mm.yyyy HH24:MI')");
	static const QString sqliteSelect = select.arg("strftime('%d.%m.%Y %H:%M', e.examinationDate)");
	static const QString from  =" FROM Examination e "
								" LEFT JOIN Patient p ON e.patientId = p.id ";
	static const QString order =" ORDER BY e.examinationDate DESC";


	QString query;

	switch(Therapeutist::interfaces->db->currentSqlDriver())
	{
	case DatabaseInterface::PSQL:
		query = psqlSelect;
		break;
	case DatabaseInterface::SQLITE:
		query = sqliteSelect;
		break;
	default:
		qFatal("Unknown sql driver");
	}

	query += from + order;

	return query;
}


int MainTherapeutistWidget::examIdToPatientId(const int examId)
{
	QSqlQuery q;
	q.prepare("SELECT patientId FROM Examination WHERE id = ?");
	q.addBindValue(examId);
	q.exec();
	checkQuery(q);

	int patientId = SaveablePluginWidget::InvalidId;
	if(q.first() && !q.value(0).isNull())
		patientId = q.value(0).toInt();

	return patientId;
}


void MainTherapeutistWidget::updateExaminationList()
{
	m_queryModel->setQuery(examinationListQuery());
	checkQuery(m_queryModel->query());
}


int MainTherapeutistWidget::numberOfSelectedExams() const
{
	return m_examinations->selectionModel()->selectedRows(0).size();
}


int MainTherapeutistWidget::selectedExamId() const
{
	int id = ExaminationEditWidget::InvalidId;

	if(numberOfSelectedExams() == 1)
	{
		const int row = m_examinations->currentIndex().row();
		id = m_queryModel->record(row).value(0).toInt();
	}

	return id;
}


void MainTherapeutistWidget::examSelectionChanged()
{
	const bool canChangeExam = Therapeutist::interfaces->usr->currentUserIsAdmin() ||
							 currentUserIsAssociatedWithTherapeutist();
	const bool singleExamSelected = (numberOfSelectedExams() == 1);


	m_addExam->setEnabled(canChangeExam);
	m_editExam->setEnabled(canChangeExam && singleExamSelected);
	m_deleteExam->setEnabled(canChangeExam && singleExamSelected);
	m_openPreview->setEnabled(singleExamSelected);
}


void MainTherapeutistWidget::addExam()
{
	const int examId = ExaminationEditWidget::InvalidId;
	ExaminationEditWidget* exam = new ExaminationEditWidget(examId, this);

	emit requestToAddNewWidget(exam, exam->tabName(examIdToPatientId(examId)));
	connect(exam, SIGNAL(saved()), SLOT(updateExaminationList()));
}


void MainTherapeutistWidget::editExam()
{
	const int examId = selectedExamId();
	ExaminationEditWidget* exam = new ExaminationEditWidget(selectedExamId(), this);

	emit requestToAddNewWidget(exam, exam->tabName(examIdToPatientId(examId)));
	connect(exam, SIGNAL(saved()), SLOT(updateExaminationList()));
}


void MainTherapeutistWidget::deleteExam()
{
	const QString& title = "Удаление осмотра пациента";
	const QString& descr = "Вы действительно хотите удалить осмотр пациента? "
						   "Все данные об осмотре также будут удалены.";

	const int rc = QMessageBox::question(this, title, descr,
										 QMessageBox::Yes | QMessageBox::No);

	if(rc == QMessageBox::Yes)
	{
		const int examinationId = selectedExamId();

		QSqlQuery q;
		q.prepare(" DELETE FROM ExaminationData WHERE examinationId = ?");
		q.addBindValue(examinationId);
		q.exec();
		checkQuery(q);

		q.prepare("DELETE FROM Examination WHERE id = ?");
		q.addBindValue(examinationId);
		q.exec();
		checkQuery(q);

		updateExaminationList();
	}
}


void MainTherapeutistWidget::openExamPreview()
{
	const int examId = selectedExamId();

	ExaminationPreview* preview = new ExaminationPreview(selectedExamId());
	emit requestToAddNewWidget(preview,
							   ExaminationEditWidget::tabName(examIdToPatientId(examId)));
}


void MainTherapeutistWidget::openExamListReport()
{
	ExaminationListReport* report = new ExaminationListReport(this);
	emit requestToAddNewWidget(report, "Отчет об осмотрах");
}


bool MainTherapeutistWidget::currentUserIsAssociatedWithTherapeutist()
{
	QSqlQuery q;
	q.prepare(" SELECT '1' "
			  " FROM MUser u "
			  " LEFT JOIN Staff s ON u.attachedStaffId = s.id "
			  " LEFT JOIN DepartmentStaffPosition dsp ON s.id = dsp.staffId "
			  " LEFT JOIN Position p ON dsp.positionId = p.id "
			  " WHERE u.id = ? AND p.textid = ?");
	q.addBindValue(Therapeutist::interfaces->usr->currentUserId());
	q.addBindValue("therapeutist");
	q.exec();
	checkQuery(q);

	return q.first();
}
