#include "searchwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>

#include <QVariant>
#include <QStyle>


SearchWidget::SearchWidget(QWidget *parent)
	: QWidget(parent)
	, m_label(new QLabel(this))
	, m_lineEdit(new QLineEdit(this))
	, m_searchButton(new QToolButton(this))
	, m_clearButton(new QToolButton(this))
{
	init();
	initConnections();
}


void SearchWidget::init()
{
	if(m_label->text().isEmpty())
		m_label->hide();

	m_clearButton->setProperty("type", "cancel");
	m_searchButton->setProperty("type", "spyglass");


	// Чтобы строка текста в QLineEdit не наезжала на кнопку сброса.
	const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	const int paddingRight = m_clearButton->sizeHint().width() + frameWidth + 1;
	m_lineEdit->setStyleSheet(QString("padding-right: %1px;").arg(paddingRight));


	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_label);
	layout->addWidget(m_lineEdit);
	layout->addWidget(m_searchButton);

	// Убираем поля слева, справа, снизу и сверху.
	layout->setContentsMargins(QMargins());
	setLayout(layout);
}


void SearchWidget::initConnections()
{
	connect(m_searchButton, SIGNAL(clicked()), SIGNAL(searchPressed()));
	connect(m_clearButton, SIGNAL(clicked()), m_lineEdit, SLOT(clear()));
	connect(m_clearButton, SIGNAL(clicked()), SIGNAL(searchPressed()));
	connect(m_lineEdit, SIGNAL(returnPressed()), SIGNAL(searchPressed()));
}


void SearchWidget::resizeEvent(QResizeEvent*)
{
	const QSize& size = m_clearButton->sizeHint();
	const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	m_clearButton->move(m_lineEdit->rect().right() - frameWidth - size.width(),
						(rect().bottom() + 1 - size.height())/2);

}


void SearchWidget::setLabelText(const QString &labelText)
{
	m_label->setText(labelText);
	m_label->setHidden(labelText.isEmpty());
}


QString SearchWidget::enteredText() const
{
	return m_lineEdit->text();
}
