#include "examinationeditwidget.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "macros.h"
#include "examcontainer.h"



ExaminationEditWidget::ExaminationEditWidget(const int examinationId, QWidget *parent)
	: SaveablePluginWidget(parent)
	, m_examinationId(examinationId)
{
	setupUi(this);
	init();

	Q_ASSERT(InvalidId == ExamWidget::InvalidId);
}


void ExaminationEditWidget::init()
{
	ExamContainer* cont = new ExamContainer(ExamWidget::InvalidId, "main", QString::null);
	m_widgetsLayout->addWidget(cont);
}


bool ExaminationEditWidget::canSave(QString &errorDescription) const
{
	Q_UNUSED(errorDescription);
	// todo
	return false;
}


void ExaminationEditWidget::save()
{
	// todo
}
