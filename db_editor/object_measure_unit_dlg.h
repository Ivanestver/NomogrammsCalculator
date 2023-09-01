#pragma once
#include "ui_object_measure_unit_dlg.h"
#include <QDialog>
#include <QObject>
#include <QUuid>

class DlgObjectMeasureUnit : public QDialog
{
	Q_OBJECT;
	using MeasureUnitInfo = std::pair<QUuid, QString>;

public:
	DlgObjectMeasureUnit(const QUuid& objId_, QWidget* parent = nullptr);
	~DlgObjectMeasureUnit() override = default;

private:
	void fillLists();
	void fillAllMeasuresUnits();
	void fillMeasuresUnitsOfObj();
	void intersectVectors(const std::vector<MeasureUnitInfo>& intersected, std::vector<MeasureUnitInfo>& reduced);
	void sortVector(std::vector<MeasureUnitInfo>& v);
	void sortMeasureUnitInfoVectors();

private:
	template<class InputIt1, class InputIt2, class OutputIt>
	OutputIt set_difference(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2, OutputIt d_first)
	{
		while (first1 != last1)
		{
			if (first2 == last2)
				return std::copy(first1, last1, d_first);

			if (*first1 < *first2)
				*d_first++ = *first1++;
			else
			{
				if (!(*first2 < *first1))
					++first1;
				++first2;
			}
		}
		return d_first;
	}

	void setDifference(const std::vector<MeasureUnitInfo>& v1, const std::vector<MeasureUnitInfo>& v2, std::vector<MeasureUnitInfo>& diff)
	{
		for (size_t i = 0, j = 0; i < v1.size();)
		{
			if (j == v2.size())
			{
				diff.insert(diff.begin(), std::next(v1.begin(), i), v1.end());
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
	Ui::DlgObjectMeasureUnit ui;
	QUuid objId;
	std::vector<MeasureUnitInfo> measureInfoVector;
	std::vector<MeasureUnitInfo> measureInfoVectorOrigin;
	std::vector<MeasureUnitInfo> measureInfoOfObjVector;
	std::vector<MeasureUnitInfo> measureInfoOfObjVectorOrigin;
};