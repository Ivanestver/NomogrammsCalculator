#include "choose_item_type_dlg.h"

DlgChooseItemType::DlgChooseItemType(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
}

bool DlgChooseItemType::IsNomogramm() const
{
	return m_ui.nomogrammRadioBtn->isChecked();
}
