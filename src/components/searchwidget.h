#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class SearchWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SearchWidget(QWidget* parent = 0);
	void setLabelText(const QString& labelText);

	QString enteredText() const;

private:
	void init();
	void initConnections();

	QLabel* m_label;
	QLineEdit* m_lineEdit;
	QPushButton* m_searchButton;
	QPushButton* m_clearButton;

signals:
	void searchPressed();

};
