#include "test_nn.h"
#include <QListWidget>

namespace ui
{
	TestNNDlg::TestNNDlg(const TestNNInfo& info_, QWidget* parent_)
		: QDialog(parent_)
		, info(info_)
	{
		ui.setupUi(this);
		ui.inputValuesTable->setRowCount(info.inputParamsNumber);
		ui.inputValuesTable->setColumnCount(2);
		ui.inputValuesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

		ui.outputValuesTable->setRowCount(info.outputParamsNumber);
		ui.outputValuesTable->setColumnCount(1);
		ui.outputValuesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

		ui.nnNameLineEdit->setText(info.nnName);

		showInputParams();

		connect(ui.calcBtn, &QPushButton::clicked, this, &TestNNDlg::onCalcBtnClicked);
		connect(ui.showNNStructureBtn, &QPushButton::clicked, this, &TestNNDlg::onShowStructureNNBtnClicked);
	}

	void TestNNDlg::showInputParams() noexcept
	{
		for (int i = 0; i < info.inputParamsNumber; ++i)
		{
			auto* item = new QTableWidgetItem(QString::fromLocal8Bit("Входной параметр №%1").arg(i + 1));
			item->setFlags(item->flags() ^ Qt::ItemIsEditable);
			ui.inputValuesTable->setItem(i, 0, item);
			ui.inputValuesTable->setItem(i, 1, new QTableWidgetItem("0"));
		}
	}

	std::vector<double> TestNNDlg::gatherInputData() const noexcept
	{
		std::vector<double> inputData;
		for (int i = 0; i < info.inputParamsNumber; ++i)
		{
			const auto* item = ui.inputValuesTable->item(i, 1);
			double value = item->text().toDouble();
			inputData.push_back(value);
		}
		return inputData;
	}

	at::Tensor TestNNDlg::makeTensorFromVector(const std::vector<double>& values) const
	{
		auto tensor = torch::zeros({ (long long)values.size(), 1LL });
		for (size_t i = 0; i < values.size(); ++i)
			tensor[i][0] = values[i];

		return tensor;
	}

	void TestNNDlg::showResult(const at::Tensor& result)
	{
		for (size_t i = 0; i < result.size(0); ++i)
		{
			double value = result[i][0].item().toDouble();
			ui.outputValuesTable->setItem(i, 0, new QTableWidgetItem(QString::number(value)));
		}
	}

	void TestNNDlg::onShowStructureNNBtnClicked()
	{
		QStringList modules;
		info.nn->Print(modules);

		QDialog dlg(this);

		auto* listWidget = new QListWidget();
		for (const auto& module : modules)
			listWidget->addItem(module);

		auto* layout = new QHBoxLayout();
		layout->addWidget(listWidget);
		dlg.setLayout(layout);

		dlg.exec();
	}

	void TestNNDlg::onCalcBtnClicked()
	{
		auto inputData = gatherInputData();
		auto result = info.nn->Predict(makeTensorFromVector(inputData));
		showResult(result);
	}
}
