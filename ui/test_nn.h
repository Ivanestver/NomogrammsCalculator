#pragma once

#include <QDialog>
#include "ml/AbstractNN.h"
#include <QWidget>
#include "ui/ui_test_nn.h"
#include <vector>

namespace ui
{
	struct TestNNInfo
	{
		QString nnName{ "" };
		std::shared_ptr<ml::FullyConnectedNN> nn{ nullptr };
		int inputParamsNumber{ 0 };
		int outputParamsNumber{ 0 };
	};

	class TestNNDlg : public QDialog
	{
		Q_OBJECT;

	public:
		TestNNDlg(const TestNNInfo& info_, QWidget* parent_ = nullptr);
		~TestNNDlg() override = default;

	private:
		void showInputParams() noexcept;
		std::vector<double> gatherInputData() const noexcept;
		at::Tensor makeTensorFromVector(const std::vector<double>& values) const;
		void showResult(const at::Tensor& result);

	private Q_SLOTS:
		void onCalcBtnClicked();
		void onShowStructureNNBtnClicked();

	private:
		Ui::TestNNDialog ui;
		TestNNInfo info;
		int a;
	};
}