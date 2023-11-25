#include "set_bypass_rule_args_dlg.h"
#include "set_bypass_rule_args_model.h"

DlgSetBypassRuleArgs::DlgSetBypassRuleArgs(const QUuid& id, QWidget* parent)
	: QDialog(parent)
	, m_ui(new Ui::DlgSetBypassRuleArgs())
{
	m_ui->setupUi(this);
	m_ui->tableView->setModel(new BypassRuleArgsModel(id));
	m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
}
