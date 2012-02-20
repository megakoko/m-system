#pragma once


#include <QObject>

class QWidget;
class QLabel;
class QToolButton;
class QComboBox;
class QLineEdit;
class QDoubleSpinBox;

class RuleItem : public QObject
{
	Q_OBJECT
public:
	RuleItem();

	QLabel* symptomLabel() const;
	QToolButton* symptomButton() const;
	QComboBox* itemOperator() const;
	QWidget* value() const;
	QToolButton* removeButton() const;

	bool canSave() const;
	void save();

private:
	void init();
	void initConnections();

	int m_itemId;

	QLabel* m_symptomLabel;
	QToolButton* m_symptomButton;
	QComboBox* m_itemOperator;
	QWidget* m_value;
	QLineEdit* m_textValue;
	QDoubleSpinBox* m_realValue;
	QComboBox* m_enumValue;
	QToolButton* m_removeButton;

private slots:


signals:
	void removeMe();

};
