#include "measures_list_dlg.h"
#include <QMenuBar>
#include "db_executor.h"
#include <QMessageBox>
#include <QInputDialog>

DlgMeasuresUnitsList::DlgMeasuresUnitsList(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	addMenuBar();
	fillTables();
}

void DlgMeasuresUnitsList::addMenuBar()
{
	auto* menuBar = new QMenuBar();
	layout()->setMenuBar(menuBar);

	menuBar->addAction(QString::fromLocal8Bit("Добавить новый тип данных"), this, &DlgMeasuresUnitsList::onAddNewTypeBtnClicked);
	menuBar->addAction(QString::fromLocal8Bit("Добавить новую единицу измерения"), this, &DlgMeasuresUnitsList::onAddNewUnitBtnClicked);
	menuBar->addAction(QString::fromLocal8Bit("Добавить новый тип данных с единицей измерения"), this, &DlgMeasuresUnitsList::onAddNewTypeUnitBtnClicked);
}

void DlgMeasuresUnitsList::fillTables()
{
	fillTypesTable();
	fillUnitTable();
	fillTypeUnitTable();
}

void DlgMeasuresUnitsList::fillTypesTable()
{
	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "select * from measure";
	DBExecutor::Response response;
	QString error;
	if (!db->ExecSELECT(queryString, {}, response, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}
	
	ui.listMeasures->clear();
	measures.clear();
	for (const auto& record : response)
	{
		ItemModel item(record[0].toUuid(), record[1].toString());
		measures.push_back(item);
		ui.listMeasures->addItem(item.name);
	}
}

void DlgMeasuresUnitsList::fillUnitTable()
{
	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "select * from unit";
	DBExecutor::Response response;
	QString error;
	if (!db->ExecSELECT(queryString, {}, response, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}

	ui.listUnits->clear();
	units.clear();
	for (const auto& record : response)
	{
		ItemModel item(record[0].toUuid(), record[1].toString());
		units.push_back(item);
		ui.listUnits->addItem(item.name);
	}
}

void DlgMeasuresUnitsList::fillTypeUnitTable()
{
	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "select * from measure_unit";
	DBExecutor::Response response;
	QString error;
	if (!db->ExecSELECT(queryString, {}, response, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}

	ui.listMeasureUnit->clear();
	measuresUnits.clear();
	for (const auto& record : response)
	{
		MeasureUnitModel item(record[0].toUuid(), record[1].toUuid(), record[2].toUuid(), record[3].toString());
		measuresUnits.push_back(item);
		ui.listMeasureUnit->addItem(item.measureUnitName);
	}
}

void DlgMeasuresUnitsList::onAddNewUnitBtnClicked()
{
	auto text = QInputDialog::getText(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Введите название новой единицы измерения"));

	ItemModel newUnit;
	newUnit.id = QUuid::createUuid();
	newUnit.name = text;

	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "insert into unit values (?, ?)";
	QString error;
	if (!db->ExecChange(queryString, { newUnit.id, newUnit.name }, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}
	units.push_back(newUnit);
	ui.listUnits->addItem(newUnit.name);
}

void DlgMeasuresUnitsList::onAddNewTypeBtnClicked()
{
	auto text = QInputDialog::getText(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Введите название нового типа данных"));

	ItemModel newUnit;
	newUnit.id = QUuid::createUuid();
	newUnit.name = text;

	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "insert into measure values (?, ?)";
	QString error;
	if (!db->ExecChange(queryString, { newUnit.id, newUnit.name }, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}
	measures.push_back(newUnit);
	ui.listMeasures->addItem(newUnit.name);
}

void DlgMeasuresUnitsList::onAddNewTypeUnitBtnClicked()
{
	const auto selectedMeasures = ui.listMeasures->selectedItems();
	if (selectedMeasures.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Необходимо выбрать тип данных"));
		return;
	}

	const auto selectedUnits = ui.listUnits->selectedItems();
	if (selectedUnits.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Необходимо выбрать единицу измерения"));
		return;
	}

	auto text = QInputDialog::getText(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Введите название нового типа данных с единицей измерения"));

	MeasureUnitModel newUnit;
	newUnit.id = QUuid::createUuid();
	newUnit.measureId = measures[ui.listMeasures->row(selectedMeasures.first())].id;
	newUnit.unitId = units[ui.listUnits->row(selectedUnits.first())].id;
	newUnit.measureUnitName = text;

	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	QString queryString = "insert into measure_unit values (?, ?, ?, ?)";
	QString error;
	if (!db->ExecChange(queryString, { newUnit.id, newUnit.measureId, newUnit.unitId, newUnit.measureUnitName }, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
		return;
	}
	measuresUnits.push_back(newUnit);
	ui.listMeasureUnit->addItem(newUnit.measureUnitName);
}
