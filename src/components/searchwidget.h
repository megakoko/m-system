#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QToolButton;

class SearchWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SearchWidget(QWidget* parent = 0);
	void setLabelText(const QString& labelText);

	QString enteredText() const;

protected:
	void resizeEvent(QResizeEvent *);

private:
	void init();
	void initConnections();

	QLabel* m_label;
	QLineEdit* m_lineEdit;
	QToolButton* m_searchButton;
	QToolButton* m_clearButton;

signals:
	void searchPressed();

};
