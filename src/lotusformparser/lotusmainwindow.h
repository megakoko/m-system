#pragma once

#include "ui_lotusmainwindow.h"

#include <QDropEvent>
#include <QDragEnterEvent>


#include "formparser.h"


class LotusMainWindow : public QMainWindow, private Ui::LotusMainWindow
{
	Q_OBJECT
	
public:
	explicit LotusMainWindow(QWidget *parent = 0);


protected:
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

private:
	void init();

	QStringList m_files;

private slots:
	void parse();

};
