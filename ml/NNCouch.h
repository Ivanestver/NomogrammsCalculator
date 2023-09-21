#pragma once

#include "ml/AbstractNN.h"
#include "ml/criterion.h"
#include <memory>
#include <functional>
#include <QString>

namespace ml
{
	struct NNCouchSettings
	{
		NNCouchSettings() = default;

		NNCouchSettings(const NNCouchSettings& other)
		{
			nn = other.nn;
			optimizer = other.optimizer;
			criterion = other.criterion;
			epochsCount = other.epochsCount;
			batchSize = other.batchSize;
			learningRate = other.learningRate;
		}

		NNCouchSettings& operator=(const NNCouchSettings& other) = default;

		std::shared_ptr<FullyConnectedNN> nn{ nullptr };
		std::shared_ptr<torch::optim::Optimizer> optimizer{ nullptr };
		std::shared_ptr<Criterion> criterion{ nullptr };
		int epochsCount = 1;
		int batchSize = 1;
		int learningRate = 0.1;
	};

	class NNCouch
	{
	public:
		using OnMessageRepliedListener = std::function<void(const QString&)>;

	public:
		NNCouch(const NNCouchSettings& settings_);
		~NNCouch() = default;

		void Train(const at::Tensor& XTrain, const at::Tensor& YTrue, const OnMessageRepliedListener& listener);

	private:
		bool isReady(const OnMessageRepliedListener& listener) const;

	private:
		NNCouchSettings settings;
	};
}