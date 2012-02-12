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
	void createHtmlDocument();

	QString css() const;

	void addExaminationInformation(QDomElement& body);
	void addExaminationData(QDomElement& body);
	void addTherapeutistSigning(QDomElement& body);

	QStringList containerData(const QString& containerTextId) const;

	static QString examDataQuery();
	static QString examDataFromSqlRecord(const QSqlRecord& record);


	const int m_examId;

	QDomDocument m_doc;

	/// Имя проводившего осмотр терапевта.
	QString m_therapeutistFullName;

private slots:
	void updateStyle();
	void print();

};
