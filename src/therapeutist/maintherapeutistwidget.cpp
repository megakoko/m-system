#include "maintherapeutistwidget.h"
#include "examinationeditwidget.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "components/decodedpatientlistquery.h"

#include <QMessageBox>

#include "therapeutist.h"
#include "macros.h"
#include "examinationpreview.h"


MainTherapeutistWidget::MainTherapeutistWidget(QWidget* parent)
	: PluginWidget(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void MainTherapeutistWidget::init()
{
	// todo
	m_queryModel = new QSqlQueryModel(this);

	m_proxyModel = new DecodedPatientListQuery(this);
	m_proxyModel->setInterfacesPtr(Therapeutist::interfaces);
	m_proxyModel->addColumnToDecode(2);
	m_proxyModel->addColumnToDecode(3);
	m_proxyModel->addColumnToDecode(4);
	m_proxyModel->setModel(m_queryModel);

	updateExaminationList();

	m_examinations->setModel(m_proxyModel);
	m_examinations->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_examinations->setColumnHidden(0, true);

	m_editExam->setEnabled(false);
	m_deleteExam->setEnabled(false);
	m_openPreview->setEnabled(false);
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
}


QString MainTherapeutistWidget::examinationListQuery()
{
	static const QString psqlSelect = /* Предложение SELECT для PostgreSQL */
								" SELECT e.id, to_char(e.examinationDate, 'dd.mm.yyyy HH24:MI'), "
								" p.familyName, p.name, p.patronymic ";
	static const QString sqliteSelect = /* Предложение SELECT для SQLite */
								" SELECT e.id, strftime('%d.%m.%Y %H:%M', e.examinationDate), "
								" p.familyName, p.name, p.patronymic ";
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
	const bool disableButtons = (numberOfSelectedExams() != 1);

	m_editExam->setDisabled(disableButtons);
	m_deleteExam->setDisabled(disableButtons);
	m_openPreview->setDisabled(disableButtons);
}


void MainTherapeutistWidget::addExam()
{
	const int examinationId = ExaminationEditWidget::InvalidId;
	ExaminationEditWidget* exam = new ExaminationEditWidget(examinationId, this);

	emit requestToAddNewWidget(exam, "TODO");
	connect(exam, SIGNAL(saved()), SLOT(updateExaminationList()));
}


void MainTherapeutistWidget::editExam()
{
	ExaminationEditWidget* exam = new ExaminationEditWidget(selectedExamId(), this);

	emit requestToAddNewWidget(exam, "TODO");
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
	ExaminationPreview* preview = new ExaminationPreview(selectedExamId());
	emit requestToAddNewWidget(preview, "TODO");
}
