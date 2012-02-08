#pragma once

#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	PlainTextEdit(QWidget* parent = NULL);

private:
	int requiredHeight() const;
	int m_lastHeight;

private slots:
	void resizeWidget();

};
