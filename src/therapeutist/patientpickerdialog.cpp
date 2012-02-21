#include "patientpickerdialog.h"

#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "components/decodingproxymodel.h"
#include "components/columnjoiningproxymodel.h"
#include "components/yosortfilterproxymodel.h"

#include "therapeutist.h"
#include "saveablepluginwidget.h"
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
	proxy->addEncodedStringColumn(1);
	proxy->addEncodedStringColumn(2);
	proxy->addEncodedStringColumn(3);
	proxy->addEncodedDateColumn(4);
	proxy->setModel(m_queryModel);

	ColumnJoiningProxyModel* columnJoining = new ColumnJoiningProxyModel(this);
	columnJoining->setJoinedColumns(1, 3, "Имя пациента");
	columnJoining->setModel(proxy);

	m_sortModel = new YoSortFilterProxyModel(this);
	m_sortModel->setSourceModel(columnJoining);
	m_sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_sortModel->setFilterKeyColumn(1);

	updatePatientsList();

	m_patientTable->setModel(m_sortModel);
	m_patientTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	m_patientTable->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
	m_patientTable->setColumnHidden(0, true);

	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	m_searchWidget->setLabelText("Поиск по фамилии:");
}


void PatientPickerDialog::initConnections()
{
	connect(m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(accept()));
	connect(m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));

	connect(m_patientTable, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()));

	connect(m_patientTable->selectionModel(),
			SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			SLOT(selectionChanged()));


	connect(m_searchWidget, SIGNAL(searchPressed()), SLOT(updatePatientsList()));
}


void PatientPickerDialog::updatePatientsList()
{
	m_queryModel->setQuery(patientListQuery());
	// Загружаем все записи.
	while(m_queryModel->canFetchMore())
		m_queryModel->fetchMore();

	m_sortModel->setFilterWildcard(m_searchWidget->enteredText());
	m_sortModel->sort(1);	// Колонка с фамилией.

	checkQuery(m_queryModel->query());
}


QString PatientPickerDialog::patientListQuery() const
{
	static const QString query =
			" SELECT id, familyName, name, patronymic, birthday AS \"Дата рождения\" "
			" FROM Patient "
			" ORDER BY familyName, name, patronymic ";

	return query;
}



void PatientPickerDialog::selectionChanged()
{
	const bool patientIsSelected =
			m_patientTable->selectionModel()->hasSelection() &&
			m_patientTable->selectionModel()->selectedRows().count() == 1;

	m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(patientIsSelected);
}


int PatientPickerDialog::selectedPatientId() const
{
	const QModelIndexList& indexes = m_patientTable->selectionModel()->selectedRows(0);

	int id = SaveablePluginWidget::InvalidId;
	if(indexes.size() == 1)
	{
		const int row = m_sortModel->mapToSource(indexes.first()).row();
		id = m_queryModel->record(row).value(0).toInt();
	}
	return id;
}
