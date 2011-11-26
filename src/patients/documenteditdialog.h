#pragma once

#include "ui_documenteditdialog.h"
#include "document.h"


class DocumentEditDialog : public QDialog, private Ui::DocumentEditDialog
{
    Q_OBJECT

public:
	explicit DocumentEditDialog(const Document& doc, QWidget *parent = 0);
	Document document();

private:
	Document m_document;

private slots:
	void checkFields();

};

