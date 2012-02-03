#include "maintherapeutistwidget.h"
#include "examinationeditwidget.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "patients/decodedpatientlistquery.h"

#include "therapeutist.h"
#include "macros.h"


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
}


void MainTherapeutistWidget::initConnections()
{
	connect(m_examinations->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(examSelectionChanged()));

	connect(m_examinations, SIGNAL(doubleClicked(QModelIndex)), SLOT(editExam()));

	connect(m_addExam, SIGNAL(clicked()), SLOT(addExam()));
	connect(m_editExam, SIGNAL(clicked()), SLOT(editExam()));
	connect(m_deleteExam, SIGNAL(clicked()), SLOT(editExam()));
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
}


void MainTherapeutistWidget::addExam()
{
	ExaminationEditWidget* exam =
			new ExaminationEditWidget(ExaminationEditWidget::InvalidId, this);

	addNewWidget(exam, "TODO");

	// todo
}


void MainTherapeutistWidget::editExam()
{	
	ExaminationEditWidget* exam =
			new ExaminationEditWidget(selectedExamId(), this);

	addNewWidget(exam, "TODO");

	// todo
}


void MainTherapeutistWidget::deleteExam()
{
	// todo
}
