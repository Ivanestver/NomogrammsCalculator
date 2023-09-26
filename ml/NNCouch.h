#pragma once

#include "ml/AbstractNN.h"
#include "ml/criterion.h"
#include <memory>
#include <functional>
#include <QString>
#include <QObject>
#include "ml/optimizer.h"
#include <chrono>

namespace ml
{
	struct NNCouchSettings
	{
		NNCouchSettings() = default;

		NNCouchSettings(const NNCouchSettings& other)
		{
			nn = other.nn;
			optimizerType = other.optimizerType;
			criterion = other.criterion;
			epochsCount = other.epochsCount;
			batchSize = other.batchSize;
			learningRate = other.learningRate;
		}

		NNCouchSettings& operator=(const NNCouchSettings& other) = default;

		std::shared_ptr<FullyConnectedNN> nn{ nullptr };
		OptimizerType optimizerType{ OptimizerType::Adam };
		std::shared_ptr<Criterion> criterion{ nullptr };
		int epochsCount = 1;
		int batchSize = 1;
		double learningRate = 0.1;
	};

	struct LearningReply
	{
		int epochNumber = -1;
		QString message = "";
		double avgLoss = 0.0;
	};

	struct LearningStatistics
	{
		LearningStatistics() = default;
		LearningStatistics(const LearningStatistics& other) = default;

		std::vector<double> epochLosses;
		std::vector<double> decadeLosses;
		std::chrono::milliseconds executionTime{ 0 };
	};
	
	class NNCouch : public QObject
	{
		Q_OBJECT;

	public:
		NNCouch(const NNCouchSettings& settings_);
		~NNCouch() = default;

		void Train(const at::Tensor& XTrain, const at::Tensor& YTrue);
		LearningStatistics GetStatistics() const noexcept;

	Q_SIGNALS:
		void EpochFinished(const LearningReply& reply) const;
		void DecadeFinished(const LearningReply& reply) const;
		void ErrorRaised(const QString& error) const;

	private:
		bool isReady() const;

	private:
		NNCouchSettings settings;
		std::shared_ptr<torch::optim::Optimizer> optimizer{ nullptr };
		LearningStatistics stats;
	};
}