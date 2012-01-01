#pragma once

#include "ui_positionseditdialog.h"

class PositionsEditDialog : public QDialog, private Ui::PositionsEditDialog
{
    Q_OBJECT

public:
    explicit PositionsEditDialog(QWidget *parent = 0);

	static QStringList positions();

private:
	void init();
	QString generatePositionName() const;

	QSet<int> m_markedForDeletionPositions;

private slots:
	void save();

	void addPosition();
	void deletePosition();

	void selectionChanged();

	void itemChanged(QListWidgetItem* item);

};
