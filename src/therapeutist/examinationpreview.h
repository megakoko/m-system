#pragma once

#include "ui_examinationpreview.h"
#include "pluginwidget.h"

#include <QDomDocument>


class QSqlRecord;

class ExaminationPreview : public PluginWidget, private Ui::ExaminationPreview
{
	Q_OBJECT
public:
	explicit ExaminationPreview(const int examinationId, QWidget *parent = 0);

private:
	void init();

	QString css() const;

	void addExaminationInformation(QDomElement& body);
	void addExaminationData(QDomElement& body);

	QStringList containerData(const QString& containerTextId) const;

	static QString examDataQuery();
	static QString examDataFromSqlRecord(const QSqlRecord& record);


	const int m_examId;

	QDomDocument m_doc;
	QDomElement m_style;

private slots:
	void updateStyle();

};
