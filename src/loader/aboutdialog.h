
#pragma once

#include <QDialog>

class AboutDialog : public QDialog
{
public:
	explicit AboutDialog(QWidget *parent = 0);

private:
	QString aboutText() const;
	QString licenceText() const;
};
