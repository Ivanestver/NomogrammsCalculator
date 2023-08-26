#pragma once

#include <QDialog>
#include "ui_choose_item_type_dlg.h"

class DlgChooseItemType : public QDialog
{
public:
	DlgChooseItemType(QWidget* parent = nullptr);
	DlgChooseItemType(const DlgChooseItemType&) = delete;
	~DlgChooseItemType() override = default;

	bool IsNomogramm() const;
	QString GetName() const;

private:
	Ui::ChooseItemTypeDlg ui;
};