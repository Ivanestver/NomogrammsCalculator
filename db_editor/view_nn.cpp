#include "view_nn.h"
#include <qmessagebox.h>
#include <QMenu>
#include <QListWidget>

namespace ui
{
	DlgViewNN::DlgViewNN(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		/*fillNNList();

		connect(ui.NNNamesListWidget, &QListWidget::itemClicked, this, &DlgViewNN::onCurrentNNNameChanged);
		connect(ui.testBtn, &QPushButton::clicked, this, &DlgViewNN::onTestBtnClicked);
		connect(ui.NNNamesListWidget, &QListWidget::customContextMenuRequested, this, &DlgViewNN::onShowContextMenu);*/
	}

	void DlgViewNN::onCurrentNNNameChanged(QListWidgetItem* current)
	{
		if (!current)
			return;

		fillByNetInfo();
	}

	void DlgViewNN::onTestBtnClicked()
	{
		/*if (currentNN.expired())
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Не выбрана сеть"));
			return;
		}

		auto nn = currentNN.lock();

		auto[input, output] = nn->GetInputOutputParamsCount();

		TestNNInfo info;
		info.inputParamsNumber = input;
		info.nn = nn;
		info.outputParamsNumber = output;
		info.nnName = QString::fromStdString(nn->name());
		TestNNDlg dlg(info, this);
		dlg.exec();*/
	}

	void DlgViewNN::onShowContextMenu(const QPoint& point)
	{ 
		// Handle global position
		QPoint globalPos = ui.NNNamesListWidget->mapToGlobal(point);

		// Create menu and insert some actions
		QMenu myMenu;
		myMenu.addAction(QString::fromLocal8Bit("Удалить"), this, &DlgViewNN::onRemoveContextMenuItemClicked);

		// Show context menu at handling position
		myMenu.exec(globalPos);
	}

	void DlgViewNN::onRemoveContextMenuItemClicked()
	{
		/*if (currentNN.expired())
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите элемент"));
			return;
		}

		auto selected = ui.NNNamesListWidget->selectedItems();
		if (selected.empty())
			return;

		auto* currentItem = ui.NNNamesListWidget->currentItem();
		QUuid idOfItemToRemove = currentItem->data(Qt::UserRole).toUuid();
		auto& nnStorage = ml::NNStorage::GetInstance();
		if (!nnStorage.RemoveNet(idOfItemToRemove))
		{
			QMessageBox::information(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Не удалось удалить выбранный элемент"));
			return;
		}

		auto nn = currentNN.lock();
		nn.reset();

		auto* selectedItem = ui.NNNamesListWidget->takeItem(ui.NNNamesListWidget->currentRow());
		if (selectedItem)
			delete selectedItem;

		ui.NNNameLineEdit->clear();
		ui.layoutsListWidget->clear();*/
	}

	void DlgViewNN::fillNNList()
	{
		/*auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		auto nnsInfo = db->GetNNModels();
		for (const auto& pair : nnsInfo)
		{
			auto* item = new QListWidgetItem(std::get<0>(pair));
			item->setData(Qt::UserRole, std::get<2>(pair));
			ui.NNNamesListWidget->addItem(item);
		}*/
	}

	void DlgViewNN::fillByNetInfo()
	{
		/*if (currentNN.expired())
			return;

		auto nn = currentNN.lock();
		ui.NNNameLineEdit->setText(QString::fromStdString(nn->name()));
		QStringList modules;
		nn->Print(modules);

		for (const auto& module : modules)
			ui.layoutsListWidget->addItem(module);*/
	}
}