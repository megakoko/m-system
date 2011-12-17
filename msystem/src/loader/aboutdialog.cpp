
#include "aboutdialog.h"

#include <QBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>


AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromUtf8("О программе"));

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight);
	QTabWidget* tabwidget = new QTabWidget();

	QLabel* about = new QLabel(aboutText(), tabwidget);
	about->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	about->setOpenExternalLinks(true);
	tabwidget->addTab(about, QString::fromUtf8("О программе"));

	QTextEdit* licence = new QTextEdit(tabwidget);
	licence->setPlainText(licenceText());
	licence->setReadOnly(true);
	tabwidget->addTab(licence, QString::fromUtf8("Лицензия"));

	layout->addWidget(tabwidget);
	setLayout(layout);
}



QString AboutDialog::aboutText() const
{
	return QString::fromUtf8(
		"<H1>M-System</H1>"
		"<p>"
			"2011 (c)"
		"</p>");
}


QString AboutDialog::licenceText() const
{
	QString result;

	return result;
}

