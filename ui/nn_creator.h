#pragma once

#include <QDialog>
#include <QWidget>
#include "ui/ui_nn_creator.h"
#include "ml/NNCouch.h"

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
		void onHiddenNeuronsCountChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
	
	private Q_SLOTS:
		void onChooseFileBtnClicked();
		void onViewDataBtnClicked();
		void onStartLearningBtnClicked();
		void onEpochFinished(const ml::LearningReply& reply);
		void onDecadeFinished(const ml::LearningReply& reply);
		void onErrorRaised(const QString& error);

	private:
		void updateMap();
		void setupUIForViewDataDlg(QDialog* viewDataDlg);
		std::shared_ptr<ml::FullyConnectedNN> createNN() const;
		std::shared_ptr<ml::Criterion> createCriterion() const;
		ml::OptimizerType createOptimizer() const;
		std::pair<at::Tensor, at::Tensor> splitData() const;

	private:
		Ui::DlgNNCreator ui;
		DlgNNCreatorInfo info;
		Data inputData;
	};
}