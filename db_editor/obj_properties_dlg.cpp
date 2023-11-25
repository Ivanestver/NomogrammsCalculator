#include "obj_properties_dlg.h"
#include "properties_table_model.h"
#include "property_value_delegate.h"

DlgObjProperties::DlgObjProperties(const QUuid& itemToShowId, QWidget* parent/* = nullptr*/)
	: QDialog(parent)
	, m_ui(new Ui::DlgObjProperties())
{
	m_ui->setupUi(this);
	auto* model = new PropertiesTableModel(itemToShowId);
	m_ui->tableView->setModel(model);
	auto* horizontalHeader = m_ui->tableView->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
	m_ui->tableView->setItemDelegate(new PropertyValueDelegate());
}

bool DlgObjProperties::NameIsChanged() const
{
	return dynamic_cast<const PropertiesTableModel*>(m_ui->tableView->model())->NameIsChanged();
}

QString DlgObjProperties::GetChangedName() const
{
	return dynamic_cast<const PropertiesTableModel*>(m_ui->tableView->model())->GetChangedName();
}
