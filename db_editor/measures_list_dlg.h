#pragma once

#include <QDialog>
#include <QWidget>
#include "ui_measures_list_dlg.h"
#include "qobjectdefs.h"
#include <QUuid>

class DlgMeasuresUnitsList : public QDialog
{
	struct ItemModel
	{
		ItemModel() = default;
		ItemModel(const QUuid& id_, const QString& name_)
			: id(id_), name(name_)
		{}

		QUuid id;
		QString name;
	};

	struct MeasureUnitModel
	{
		MeasureUnitModel() = default;
		MeasureUnitModel(const QUuid& id_, const QUuid& measureId_, const QUuid& unitId_, const QString& measureUnitName_)
			: id(id_), measureId(measureId_), unitId(unitId_), measureUnitName(measureUnitName_)
		{}
		
		QUuid id;
		QUuid measureId;
		QUuid unitId;
		QString measureUnitName;
	};

	Q_OBJECT;
public:
	DlgMeasuresUnitsList(QWidget* parent = nullptr);
	~DlgMeasuresUnitsList() override = default;

private:
	void addMenuBar();
	void fillTables();
	void fillTypesTable();
	void fillUnitTable();
	void fillTypeUnitTable();

private Q_SLOTS:
	void onAddNewUnitBtnClicked();
	void onAddNewTypeBtnClicked();
	void onAddNewTypeUnitBtnClicked();

private:
	Ui::DlgMeasuresUnitsList ui;
	std::vector<ItemModel> measures;
	std::vector<ItemModel> units;
	std::vector<MeasureUnitModel> measuresUnits;
};