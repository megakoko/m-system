#include "therapeutistpickerdialog.h"


#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#include "macros.h"
#include "saveablepluginwidget.h"


TherapeutistPickerDialog::TherapeutistPickerDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void TherapeutistPickerDialog::init()
{
	m_model = new QSqlQueryModel(this);
	m_model->setQuery(query());
	checkQuery(m_model->query());

	m_view->setModel(m_model);
	m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_view->setColumnHidden(0, true);

	m_ok->setEnabled(false);
}


void TherapeutistPickerDialog::initConnections()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_searchLine, SIGNAL(returnPressed()), SLOT(updateTherapeutistList()));
	connect(m_search, SIGNAL(clicked()), SLOT(updateTherapeutistList()));
	connect(m_clear, SIGNAL(clicked()), m_searchLine, SLOT(clear()));
	connect(m_clear, SIGNAL(clicked()), SLOT(updateTherapeutistList()));

	connect(m_view->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(listSelectionChanged()));
	connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()));
}


QString TherapeutistPickerDialog::query(QString searchText)
{
	static const QString q =
			" SELECT DISTINCT s.id, s.familyName || ' ' || s.name || ' ' || s.patronymic AS doctorname "
			" FROM DepartmentStaffPosition dsp "
			" LEFT JOIN Staff s ON dsp.staffId = s.id "
			" LEFT JOIN Position p ON dsp.positionId = p.id "
			" WHERE p.textid = 'therapeutist' %1 "
			" ORDER BY doctorName ";

	if(!searchText.isEmpty())
		searchText.remove("'");

	return searchText.isEmpty() ?
			q.arg("") :
			q.arg(" AND s.familyName LIKE '%" + searchText + "%' ");
}


int TherapeutistPickerDialog::selectedTherapeutistId() const
{
	int index = SaveablePluginWidget::InvalidId;

	const QModelIndexList& selectedIndexes = m_view->selectionModel()->selectedRows(0);
	if(selectedIndexes.size() == 1)
	{
		const int row = selectedIndexes.at(0).row();
		index = m_model->record(row).value(0).toInt();
	}

	return index;
}


QString TherapeutistPickerDialog::selectedTherapeutistName() const
{
	QString name;

	const QModelIndexList& selectedIndexes = m_view->selectionModel()->selectedRows(0);
	if(selectedIndexes.size() == 1)
	{
		const int row = selectedIndexes.at(0).row();
		name = m_model->record(row).value(1).toString();
	}

	return name;
}


void TherapeutistPickerDialog::updateTherapeutistList()
{
	m_model->setQuery(query(m_searchLine->text()));
	checkQuery(m_model->query());
}


void TherapeutistPickerDialog::listSelectionChanged()
{
	const QModelIndexList& selectedIndexes = m_view->selectionModel()->selectedRows(0);

	m_ok->setEnabled(selectedIndexes.size() == 1);
}
