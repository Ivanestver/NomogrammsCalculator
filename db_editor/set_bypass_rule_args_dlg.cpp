#include "set_bypass_rule_args_dlg.h"
#include "set_bypass_rule_args_model.h"

DlgSetBypassRuleArgs::DlgSetBypassRuleArgs(const QUuid& id, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::DlgSetBypassRuleArgs())
{
	ui->setupUi(this);
	ui->tableView->setModel(new BypassRuleArgsModel(id));
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
}
