#pragma once

#include <QDialog>
#include <QWidget>
#include "ui/ui_nn_creator.h"

namespace ui
{
	struct DlgNNCreatorInfo
	{
		int inputParamsNumber = 1;
		std::vector<int> hiddenLayers;
	};

	class DlgNNCreator : public QDialog
	{
		using Data = std::vector<std::vector<QString>>;
	public:
		DlgNNCreator(QWidget* parent = nullptr);
		~DlgNNCreator() override = default;

	private:
		DlgNNCreator(const DlgNNCreator&) = delete;
		DlgNNCreator& operator=(const DlgNNCreator&) = delete;

	private Q_SLOTS: // Вкладка "Создание модели"
		void onInputParamsNumberChanged(int value);
		void onLayersCountChanged(int value);
		void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
	
	private Q_SLOTS:
		void onChooseFileBtnClicked();
		void onViewDataBtnClicked();

	private:
		void updateMap();
		void setupUIForViewDataDlg(QDialog* viewDataDlg);

	private:
		Ui::DlgNNCreator ui;
		DlgNNCreatorInfo info;
		Data inputData;
	};
}