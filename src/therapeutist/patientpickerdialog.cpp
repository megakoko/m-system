#include "patientpickerdialog.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "components/decodingproxymodel.h"
#include "components/yosortfilterproxymodel.h"

#include "therapeutist.h"
#include "macros.h"

PatientPickerDialog::PatientPickerDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	init();
	initConnections();
}


void PatientPickerDialog::init()
{
	m_queryModel = new QSqlQueryModel(this);

	DecodingProxyModel* proxy = new DecodingProxyModel(this);
	proxy->setInterfacesPtr(Therapeutist::interfaces);
	proxy->addColumnToDecode(1);
	proxy->addColumnToDecode(2);
	proxy->addColumnToDecode(3);
	proxy->setModel(m_queryModel);

	m_sortModel = new YoSortFilterProxyModel(this);
	m_sortModel->setSourceModel(proxy);
	m_sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterKeyColumn(1);

	updatePatientsList();

	m_patientTable->setModel(m_sortModel);
	m_patientTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_patientTable->setColumnHidden(0, true);

	m_ok->setEnabled(false);
}


void PatientPickerDialog::initConnections()
{
	connect(m_ok, SIGNAL(clicked()), SLOT(accept()));
	connect(m_cancel, SIGNAL(clicked()), SLOT(reject()));

	connect(m_patientTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()));

	connect(m_patientTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));

	connect(m_searchLine, SIGNAL(returnPressed()), SLOT(updatePatientsList()));
	connect(m_search, SIGNAL(clicked()), SLOT(updatePatientsList()));
	connect(m_clear, SIGNAL(clicked()), m_searchLine, SLOT(clear()));
	connect(m_clear, SIGNAL(clicked()), SLOT(updatePatientsList()));
}


void PatientPickerDialog::updatePatientsList()
{
	m_queryModel->setQuery(patientListQuery());
	// Загружаем все записи.
	while(m_queryModel->canFetchMore())
		m_queryModel->fetchMore();

	m_sortModel->setFilterWildcard(m_searchLine->text());
	m_sortModel->sort(1);	// Колонка с фамилией.

	checkQuery(m_queryModel->query());
}


QString PatientPickerDialog::patientListQuery() const
{
	static const QString psqlSelect = /* Предложение SELECT для PostgreSQL */
								" SELECT id, familyName, name, patronymic, "
								" to_char(birthday, 'dd.mm.yyyy') ";
	static const QString sqliteSelect = /* Предложение SELECT для SQLite */
								" SELECT id, familyName, name, patronymic, "
								" strftime('%d.%m.%Y', birthday) ";
	static const QString from  =" FROM Patient ";
	static const QString order =" ORDER BY familyName, name, patronymic ";


	QString query;

	switch(Therapeutist::interfaces->db->currentSqlDriver())
	{
	case DatabaseInterface::PSQL:
		query = psqlSelect;
		break;
	case DatabaseInterface::SQLITE:
		query = sqliteSelect;
		break;
	default:
		qFatal("Unknown sql driver");
	}

	query += from + order;

	return query;
}



void PatientPickerDialog::selectionChanged()
{
	m_ok->setEnabled(m_patientTable->selectionModel()->hasSelection() &&
					 m_patientTable->selectionModel()->selectedRows().count() == 1);
}


int PatientPickerDialog::selectedPatientId() const
{
	const QModelIndexList& indexes = m_patientTable->selectionModel()->selectedRows(0);

	int id = InvalidId;
	if(indexes.size() == 1)
	{
		const int row = m_sortModel->mapToSource(indexes.first()).row();
		id = m_queryModel->record(row).value(0).toInt();
	}
	return id;
}
