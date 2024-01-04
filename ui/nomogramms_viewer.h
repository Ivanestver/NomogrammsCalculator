#pragma once

#include <QtWidgets/QDialog>
#include <QObject>
#include <QWidget>
#include <memory>
#include "ui/ui_nomogramms_viewer.h"
#include "db/DataBaseWrapper.h"
#include "nomogramms/ICalculeable.h"
#include <vector>
#include "nomogramms/MeasureUnit.h"
#include "nomogramms/Nomogramm.h"

namespace ui
{
	class NomogrammsViewer : public QDialog
	{
		Q_OBJECT;

		using Value = std::pair<nomogramms::SMeasureUnit, double>;
		using ValuesMap = std::map<ParameterType, std::vector<Value>>;

	public:
		NomogrammsViewer(QWidget* parent = nullptr);
		~NomogrammsViewer() = default;

	private:
		void setTree();
		void setInputTable(const ValuesMap& params);
		void addParameterTypeToInputTable(ParameterType type, const ValuesMap& parameters);
		void addMeasureUnitToInputTable(const Value& measureUnit, QTableWidget* tableWidget);
		void setValue(const QTableWidgetItem* item);
		nomogramms::IOData createInputData() const;
		void setOutputData(const nomogramms::IOData& outputData);
		void setGraphicsTabViewMode(bool show);
		void showNomogramm(const nomogramms::SNomogramm& Nomogramm);

	private:
		NomogrammsViewer(const NomogrammsViewer&) = delete;
		NomogrammsViewer& operator=(const NomogrammsViewer&) = delete;

	private Q_SLOTS:
		void onSpinBoxValueChanged(int value);
		void onCurrentItemTreeChanged(const QModelIndex& current, const QModelIndex& previous);
		void onItemChanged(QTableWidgetItem* item);
		void onCalcBtnClicked();
		void onResetBtnClicked();

	private:
		std::shared_ptr<Ui::NomogrammViewer> ui;
		ValuesMap values;
		nomogramms::SICalculeable currentCalculeable{ nullptr };
	};
}