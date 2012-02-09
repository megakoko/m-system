#pragma once

#include "ui_examinationpreview.h"
#include "pluginwidget.h"


class ExaminationPreview : public PluginWidget, private Ui::ExaminationPreview
{
	Q_OBJECT
public:
	explicit ExaminationPreview(const int examinationId, QWidget *parent = 0);

private:
	void init();

	static const QString CSS;

	const int m_examId;

};
