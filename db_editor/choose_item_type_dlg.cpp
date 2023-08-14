#include "choose_item_type_dlg.h"

DlgChooseItemType::DlgChooseItemType(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

bool DlgChooseItemType::IsNomogramm() const
{
	return ui.nomogrammRadioBtn->isChecked();
}

QString DlgChooseItemType::GetName() const
{
	return ui.lineEdit->text();
}
