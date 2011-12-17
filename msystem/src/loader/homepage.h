

#pragma once

#include <QWidget>
#include <QMap>

class QPushButton;
class QGridLayout;


class HomePage : public QWidget
{
	Q_OBJECT

public:
	explicit HomePage(QWidget *parent = 0);
	void addButton(const QString& text, const QString& textid);
	void clearButtons();

private:
	QGridLayout* m_pluginGrid;
	QMap<QObject*, QString> m_btnToTextid;

private slots:
	void buttonClicked();

signals:
	void buttonClicked(const QString& textid);

};

