#pragma once

#include "ui_examinationpreview.h"
#include "pluginwidget.h"

#include <QDomDocument>


class ExaminationPreview : public PluginWidget, private Ui::ExaminationPreview
{
	Q_OBJECT
public:
	explicit ExaminationPreview(const int examinationId, QWidget *parent = 0);

private:
	void init();
	void addExaminationInformation(QDomElement& body);

	static const QString CSS;

	const int m_examId;

	QDomDocument m_doc;

};
