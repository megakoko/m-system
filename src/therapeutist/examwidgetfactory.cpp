#include "examwidgetfactory.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "macros.h"

#include "examwidget.h"
#include "examcontainer.h"
#include "examlineedit.h"
#include "examcombobox.h"


ExamWidgetFactory::ExamWidgetFactory()
{

}


ExamWidget* ExamWidgetFactory::createWidget(const int examId,
											const QString& textid) const
{
	QSqlQuery q;
	q.prepare(" SELECT typeid, label "
			  " FROM UiElement "
			  " WHERE textid = ? ");
	q.addBindValue(textid);
	q.exec();
	checkQuery(q);


	ExamWidget* widget = NULL;
	if(q.first())
	{
		const QString& type = q.value(0).toString();
		if(type == "container")
			widget = new ExamContainer(examId, textid, q.value(1).toString());
		else if(type == "lineedit")
			widget = new ExamLineEdit(examId, textid, q.value(1).toString());
		else if(type == "combobox")
			widget = new ExamComboBox(examId, textid, q.value(1).toString());
		else
			qWarning() << "Unknown type for widget factory:" << type;
	}

	return widget;
}
