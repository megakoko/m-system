#include "maintherapeutistwidget.h"
#include "examinationeditwidget.h"


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


int MainTherapeutistWidget::selectedExamId() const
{
	int id = ExaminationEditWidget::InvalidId;
	// todo


	return id;
}


void MainTherapeutistWidget::examSelectionChanged()
{
	// todo
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
	// todo
}


void MainTherapeutistWidget::deleteExam()
{
	// todo
}
