#pragma once

#include <QDialog>
#include <memory>
#include "ui_set_bypass_rule_args_dlg.h"

class DlgSetBypassRuleArgs : public QDialog
{
public:
	DlgSetBypassRuleArgs(const QUuid& id, QWidget* parent = nullptr);
	~DlgSetBypassRuleArgs() override = default;

private:
	std::unique_ptr<Ui::DlgSetBypassRuleArgs> ui;
};