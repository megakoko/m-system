#pragma once

#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	PlainTextEdit(QWidget* parent = NULL);

	int maxLength() const;
	void setMaxLength(const int maxLength);

private:
	int requiredHeight() const;
	int m_lastHeight;

	int m_maxLength;

private slots:
	void resizeWidget();
	void chopText();

};
