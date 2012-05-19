
#include "aboutdialog.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>


AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(QString::fromUtf8("О программе"));

	QVBoxLayout* layout = new QVBoxLayout;
	QTabWidget* tabwidget = new QTabWidget();

	QLabel* about = new QLabel(aboutText(), tabwidget);
	about->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	about->setOpenExternalLinks(true);
	tabwidget->addTab(about, QString::fromUtf8("О программе"));

/*
	QTextEdit* licence = new QTextEdit(tabwidget);
	licence->setPlainText(licenceText());
	licence->setReadOnly(true);
	tabwidget->addTab(licence, QString::fromUtf8("Лицензия"));
*/

	layout->addWidget(tabwidget);


	QPushButton* closeBtn = new QPushButton("OK");
	connect(closeBtn, SIGNAL(clicked()), SLOT(accept()));

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addSpacerItem(new QSpacerItem(10, 10,
												QSizePolicy::Expanding,
												QSizePolicy::Minimum));
	buttonLayout->addWidget(closeBtn);


	layout->addLayout(buttonLayout);

	setLayout(layout);


	resize(0, 0);
}



QString AboutDialog::aboutText() const
{
	static const QChar copyright(0x00A9);	// Знак копирайта.
	static const QChar dash(0x2014);		// Длинное тире.

	return QString(
				"<h2>M-System</h2>"
				"<h3>Медицинская информационная система</h3>"
				"<p>"
					"%1 Суфиянов Вадим, Чукавин Андрей, Насырова Юлия, 2011 %2 2012 "
				"</p>").arg(copyright).arg(dash);
}


QString AboutDialog::licenceText() const
{
	QString result;

	return result;
}

