#include "examinationpreview.h"


#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "macros.h"



const QString ExaminationPreview::CSS =
		"";


ExaminationPreview::ExaminationPreview(const int examinationId, QWidget *parent)
	: PluginWidget(parent)
	, m_examId(examinationId)
{
	setupUi(this);

	init();
}


void ExaminationPreview::init()
{

}
