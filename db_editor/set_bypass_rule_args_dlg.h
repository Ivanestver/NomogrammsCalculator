#pragma once

#include <QDialog>
#include <memory>
#include "ui_set_bypass_rule_args_dlg.h"
#include "common_macros.h"

class DlgSetBypassRuleArgs : public QDialog
{
	DECLARE_NO_COPY_NO_MOVE(DlgSetBypassRuleArgs);

public:
	DlgSetBypassRuleArgs(const QUuid& id, QWidget* parent = nullptr);
	~DlgSetBypassRuleArgs() override = default;

private:
	std::unique_ptr<Ui::DlgSetBypassRuleArgs> m_ui;
};