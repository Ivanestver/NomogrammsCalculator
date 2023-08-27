#include "obj_properties_dlg.h"
#include "properties_table_model.h"
#include "property_value_delegate.h"

DlgObjProperties::DlgObjProperties(const QUuid& itemToShowId, QWidget* parent/* = nullptr*/)
	: QDialog(parent)
	, ui(new Ui::DlgObjProperties())
{
	ui->setupUi(this);
	auto* model = new PropertiesTableModel(itemToShowId);
	ui->tableView->setModel(model);
	auto* horizontalHeader = ui->tableView->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
	ui->tableView->setItemDelegate(new PropertyValueDelegate());
}

bool DlgObjProperties::NameIsChanged() const
{
	return dynamic_cast<const PropertiesTableModel*>(ui->tableView->model())->NameIsChanged();
}

QString DlgObjProperties::GetChangedName() const
{
	return dynamic_cast<const PropertiesTableModel*>(ui->tableView->model())->GetChangedName();
}
