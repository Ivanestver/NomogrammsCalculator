#pragma once

#include <QDialog>
#include <QObject>
#include <memory>
#include "ui_obj_properties_dlg.h"

class DlgObjProperties : public QDialog
{
public:
	DlgObjProperties(const QUuid& itemToShowId, QWidget* parent = nullptr);
	~DlgObjProperties() override = default;

	bool NameIsChanged() const;
	QString GetChangedName() const;

private:
	std::unique_ptr<Ui::DlgObjProperties> ui;
};