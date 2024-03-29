#include "examdiagnosisedit.h"


#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QDebug>

#include "examcontainer.h"
#include "diagnosishelpdialog.h"


ExamDiagnosisEdit::ExamDiagnosisEdit(const int examId, const QString &textid)
	: ExamLineEdit(examId, textid)
	, m_widget(new QWidget())
	, m_mainContainer(NULL)
{
	QToolButton* diagnosisHelpButton = new QToolButton;
	diagnosisHelpButton->setProperty("type", "star");
	diagnosisHelpButton->setToolTip("Помощь в постановке диагноза");
	connect(diagnosisHelpButton, SIGNAL(clicked()), SLOT(openDiagnosisHelp()));


	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_lineEdit);
	layout->addWidget(diagnosisHelpButton);
	layout->setContentsMargins(QMargins());
	m_widget->setLayout(layout);
}


QWidget* ExamDiagnosisEdit::widget() const
{
	return m_widget;
}


void ExamDiagnosisEdit::setMainContainer(ExamContainer* container)
{
	m_mainContainer = container;
}


void ExamDiagnosisEdit::openDiagnosisHelp()
{
	if(m_mainContainer != NULL)
	{
		DiagnosisHelpDialog dialog(m_mainContainer->data(), m_widget);
		if(dialog.exec() == QDialog::Accepted)
			m_lineEdit->setText(dialog.selectedDiagnosis());
	}
	else
		qDebug() << "null container";
}
