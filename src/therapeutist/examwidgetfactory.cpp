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
#include "examtextedit.h"
#include "examspinbox.h"


ExamWidgetFactory::ExamWidgetFactory()
{

}


ExamWidget* ExamWidgetFactory::createWidget(const int examId,
											const QString& textid) const
{
	QSqlQuery q;
	q.prepare(" SELECT typeid, label, shortLabel "
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
			widget = new ExamContainer(examId, textid);
		else if(type == "lineedit")
			widget = new ExamLineEdit(examId, textid);
		else if(type == "combobox")
			widget = new ExamComboBox(examId, textid);
		else if(type == "textedit")
			widget = new ExamTextEdit(examId, textid);
		else if(type == "spinbox")
			widget = new ExamSpinBox(examId, textid);
		else
			qWarning() << "Unknown type for widget factory:" << type;

		if(widget != NULL)
			widget->setLabelText(q.value(1).toString(), q.value(2).toString());
	}

	return widget;
}
