#include "property_value_delegate.h"
#include "properties.h"
#include <QPushButton>
#include "db_objs_state.h"
#include "properties_table_model.h"

PropertyValueDelegate::PropertyValueDelegate(QObject* parent/* = nullptr*/)
	: QStyledItemDelegate(parent)
{}

QWidget* PropertyValueDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto* model = dynamic_cast<const PropertiesTableModel*>(index.model());
	if (!model)
		return nullptr;

	const auto& item = model->GetTableItemByIdx(index);
	if (item.id == db_state::properties::nomogramm_bypassRule_List)
	{
		auto* btn = new QPushButton(parent);
		btn->setText(QString::fromLocal8Bit("Открыть редактор правила обхода"));
		connect(btn, &QPushButton::clicked, this, &PropertyValueDelegate::onOpenBypassRuleListClicked);
		return btn;
	}
	else
	{
		return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

void PropertyValueDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (!dynamic_cast<QPushButton*>(editor))
		QStyledItemDelegate::setModelData(editor, model, index);
}

void PropertyValueDelegate::onOpenBypassRuleListClicked()
{

}