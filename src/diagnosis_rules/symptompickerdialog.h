#pragma once

#include "pluginwidget.h"
#include "ui_symptompickerdialog.h"


class QSqlRecord;
class QSqlQueryModel;

class SymptomPickerDialog : public QDialog, private Ui::SymptomPickerDialog
{
	Q_OBJECT

public:
	explicit SymptomPickerDialog(QWidget *parent = 0);

	int selectedSymptom() const;

private:
	void init();
	void initTreeWidget();
	void initConnections();

	static QTreeWidgetItem* createItem(const QSqlRecord& rec);
	static QList<QTreeWidgetItem*> createItems(const QVariant& parentId = QVariant());


	static const QString filterText;
	QSqlQueryModel* m_filterModel;


private slots:
	void treeWidgetItemExpanded(QTreeWidgetItem* item);
	void filterSymptoms();
	void selectionChanged();

};
