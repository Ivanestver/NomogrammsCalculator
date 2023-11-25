#include "object_measure_unit_dlg.h"
#include "db_executor.h"
#include <QMessageBox>
#include <algorithm>

DlgObjectMeasureUnit::DlgObjectMeasureUnit(const QUuid& objId_, bool isInput, QWidget* parent)
	: QDialog(parent)
	, objId(objId_)
{
	m_ui.setupUi(this);

	tableName = isInput ? "template_measure_unit_input" : "template_measure_unit_output";
	
	connect(m_ui.addMeasureUnit, &QPushButton::clicked, this, &DlgObjectMeasureUnit::onAddMeasureUnitBtnClicked);
	connect(m_ui.removeMeasureUnit, &QPushButton::clicked, this, &DlgObjectMeasureUnit::onRemoveMeasureUnitBtnClicked);
	connect(m_ui.buttonBox, &QDialogButtonBox::accepted, this, &DlgObjectMeasureUnit::onAccept);
	connect(m_ui.buttonBox, &QDialogButtonBox::rejected, this, &DlgObjectMeasureUnit::onReject);

	auto db = DBExecutor::GetInstance();
	if (db)
	{
		QString queryString = "select measure_unit_id, measure_unit_name from measure_unit";
		DBExecutor::Response response;
		QString error;
		if (!db->ExecSELECT(queryString, {}, response, error))
		{
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), error);
			return;
		}

		for (const auto& record : response)
		{
			measureInfoVector.push_back({ record[0].toUuid(), record[1].toString() });
			measureInfoVectorOrigin.push_back({ record[0].toUuid(), record[1].toString() });
		}
		sortVector(measureInfoVector);

		queryString = QString("select t2.measure_unit_id, t2.measure_unit_name from %1 as t1 inner join measure_unit as t2 on t1.measure_unit_id=t2.measure_unit_id where template_id = ?").arg(tableName);
		response.clear();
		db->ExecSELECT(queryString, { objId }, response, error);

		for (const auto& record : response)
		{
			measureInfoOfObjVector.push_back({ record[0].toUuid(), record[1].toString() });
			measureInfoOfObjVectorOrigin.push_back({ record[0].toUuid(), record[1].toString() });
		}

		intersectVectors(measureInfoOfObjVector, measureInfoVector);
		sortMeasureUnitInfoVectors();
		fillLists();
	}
}

void DlgObjectMeasureUnit::fillLists()
{
	fillAllMeasuresUnits();
	fillMeasuresUnitsOfObj();
}

void DlgObjectMeasureUnit::fillAllMeasuresUnits()
{
	m_ui.listAvailableMeasuresUnits->clear();
	for (const auto& info : measureInfoVector)
	{
		m_ui.listAvailableMeasuresUnits->addItem(info.second);
	}
}

void DlgObjectMeasureUnit::fillMeasuresUnitsOfObj()
{
	m_ui.listChosenMeasuresUnits->clear();
	for (const auto& info : measureInfoOfObjVector)
	{
		m_ui.listChosenMeasuresUnits->addItem(info.second);
	}
}

void DlgObjectMeasureUnit::intersectVectors(const std::vector<MeasureUnitInfo>& intersected, std::vector<MeasureUnitInfo>& reduced)
{
	const auto it = std::remove_if(reduced.begin(), reduced.end(), [&intersected](const MeasureUnitInfo& info)
		{
			return std::find_if(intersected.begin(), intersected.end(), [&info](const MeasureUnitInfo& info_)
				{
					return info.second == info_.second;
				}) != intersected.end();
		});

	reduced.erase(it, reduced.end());
}

void DlgObjectMeasureUnit::sortVector(std::vector<MeasureUnitInfo>& v)
{
	std::sort(v.begin(), v.end(), [](const MeasureUnitInfo& left, const MeasureUnitInfo& right)
		{
			return left.second < right.second;
		});
}

void DlgObjectMeasureUnit::sortMeasureUnitInfoVectors()
{
	sortVector(measureInfoVector);
	sortVector(measureInfoOfObjVector);
}

void DlgObjectMeasureUnit::onAddMeasureUnitBtnClicked()
{
	const auto selectedItems = m_ui.listAvailableMeasuresUnits->selectionModel()->selectedIndexes();
	if (selectedItems.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("Необходимо выбрать элемент(-ы)"));
		return;
	}
	
	for (const auto& selectedItem : selectedItems)
	{
		int rowNumber = selectedItem.row();
		measureInfoOfObjVector.push_back(measureInfoVector[rowNumber]);
		measureInfoVector.erase(std::next(measureInfoVector.begin(), rowNumber));
	}
	sortMeasureUnitInfoVectors();
	fillLists();
}

void DlgObjectMeasureUnit::onRemoveMeasureUnitBtnClicked()
{
	const auto selectedItems = m_ui.listChosenMeasuresUnits->selectionModel()->selectedIndexes();
	if (selectedItems.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("Необходимо выбрать элемент(-ы)"));
		return;
	}
	
	for (const auto& selectedItem : selectedItems)
	{
		int rowNumber = selectedItem.row();
		measureInfoVector.push_back(measureInfoOfObjVector[rowNumber]);
		measureInfoOfObjVector.erase(std::next(measureInfoOfObjVector.begin(), rowNumber));
	}
	sortMeasureUnitInfoVectors();
	fillLists();
}

void DlgObjectMeasureUnit::onAccept()
{
	std::vector<MeasureUnitInfo> difference;
	if (measureInfoOfObjVectorOrigin.empty())
		difference = measureInfoOfObjVector;
	else if (measureInfoOfObjVector.empty())
		difference = measureInfoOfObjVectorOrigin;
	else
	{
		sortVector(measureInfoOfObjVector);
		sortVector(measureInfoOfObjVectorOrigin);
		if (measureInfoOfObjVector.size() > measureInfoOfObjVectorOrigin.size())
			setDifference(measureInfoOfObjVector, measureInfoOfObjVectorOrigin, difference);
		else
			setDifference(measureInfoOfObjVectorOrigin, measureInfoOfObjVector, difference);

		if (difference.empty())
		{
			difference.insert(difference.begin(), measureInfoOfObjVector.begin(), measureInfoOfObjVector.end());
			difference.insert(difference.begin(), measureInfoOfObjVectorOrigin.begin(), measureInfoOfObjVectorOrigin.end());
		}
	}

	std::vector<MeasureUnitInfo> vectorToAdd, vectorToDelete;
	for (const auto& item : difference)
	{
		if (std::find(measureInfoOfObjVectorOrigin.begin(), measureInfoOfObjVectorOrigin.end(), item) == measureInfoOfObjVectorOrigin.end())
		{
			vectorToAdd.push_back(item);
		}
		else
		{
			vectorToDelete.push_back(item);
		}
	}
	
	auto db = DBExecutor::GetInstance();
	if (!db)
		return;

	if (!vectorToDelete.empty())
	{
		QString queryString = QString("delete from %1 where template_id = ? and measure_unit_id in (").arg(tableName);
		for (size_t i = 0; i < vectorToDelete.size(); i++)
		{
			queryString += "?,";
		}
		queryString = queryString.left(queryString.length() - 1) + ")";
		std::vector<QVariant> params { objId };
		for (const auto& item : vectorToDelete)
			params.push_back(item.first);

		QString error;
		if (!db->ExecChange(queryString, params, error))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("При удалении произошла следующая ошибка \n") + error);
		}
	}

	if (!vectorToAdd.empty())
	{
		for (const auto& item : vectorToAdd)
		{

			QString queryString = QString("insert into %1(template_id, measure_unit_id) values (?,?)").arg(tableName);
			std::vector<QVariant> params { objId, item.first };
			QString error;
			if (!db->ExecChange(queryString, params, error))
			{
				QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("При удалении произошла следующая ошибка \n") + error);
			}
		}
	}

	close();
}

void DlgObjectMeasureUnit::onReject()
{
	close();
}
