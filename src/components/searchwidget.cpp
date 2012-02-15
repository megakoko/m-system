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


	m_layout = new QHBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_lineEdit);
	m_layout->addWidget(m_searchButton);

	// Убираем поля слева, справа, снизу и сверху.
	m_layout->setContentsMargins(QMargins());
	setLayout(m_layout);
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


	const int x = (m_label->isHidden() ? 0 : m_label->width() + m_layout->spacing()) +
				  m_lineEdit->width() - frameWidth - size.width();
	const int y = (height() + 1 - size.height())/2;

	m_clearButton->move(x, y);

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
