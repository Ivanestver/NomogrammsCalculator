#pragma once
#include "ui_object_measure_unit_dlg.h"
#include <QDialog>
#include <QObject>
#include <QUuid>
#include <QString>
#include "common_macros.h"

class DlgObjectMeasureUnit : public QDialog
{
	Q_OBJECT;
	DECLARE_NO_COPY_NO_MOVE(DlgObjectMeasureUnit);

	using MeasureUnitInfo = std::pair<QUuid, QString>;

public:
	DlgObjectMeasureUnit(const QUuid& objId_, bool isInput, QWidget* parent = nullptr);
	~DlgObjectMeasureUnit() override = default;

private:
	void fillLists();
	void fillAllMeasuresUnits();
	void fillMeasuresUnitsOfObj();
	void intersectVectors(const std::vector<MeasureUnitInfo>& intersected, std::vector<MeasureUnitInfo>& reduced);
	void sortVector(std::vector<MeasureUnitInfo>& v);
	void sortMeasureUnitInfoVectors();

private:
	void setDifference(const std::vector<MeasureUnitInfo>& v1, const std::vector<MeasureUnitInfo>& v2, std::vector<MeasureUnitInfo>& diff)
	{
		for (size_t i = 0, j = 0; i < v1.size();)
		{
			if (j == v2.size())
			{
				diff.insert(diff.begin(), std::next(v1.begin(), (int)i), v1.end());
				return;
			}

			if (v1[i] < v2[j])
			{
				diff.push_back(v1[i]);
				i++;
			}
			else
			{
				if (!(v1[i] < v2[j]))
					i++;
				j++;
			}
		}
	}

private Q_SLOTS:
	void onAddMeasureUnitBtnClicked();
	void onRemoveMeasureUnitBtnClicked();
	void onAccept();
	void onReject();

private:
	Ui::DlgObjectMeasureUnit m_ui;
	QUuid objId;
	std::vector<MeasureUnitInfo> measureInfoVector;
	std::vector<MeasureUnitInfo> measureInfoVectorOrigin;
	std::vector<MeasureUnitInfo> measureInfoOfObjVector;
	std::vector<MeasureUnitInfo> measureInfoOfObjVectorOrigin;
	QString tableName = "";
};