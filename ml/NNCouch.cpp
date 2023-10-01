#include "NNCouch.h"
#include "ml/optimizer.h"
#include <chrono>

using namespace std::chrono;

namespace ml
{
	NNCouch::NNCouch(const NNCouchSettings& settings_)
		: settings(settings_)
	{
		auto optimizerInitializer = OptimizerInitializer::CreateInitializer(settings.optimizerType);
		optimizer = optimizerInitializer->CreateOptimizer(settings.learningRate, *settings.nn);
	}

	void NNCouch::Train(const at::Tensor& XTrain, const at::Tensor& YTrue, std::pair<at::Tensor, at::Tensor>&& validationSet)
	{
		if (!isReady())
			return;

		if (XTrain.sizes().empty() || YTrue.sizes().empty())
		{
			ErrorRaised(QString::fromLocal8Bit("Входные данные не валидны"));
			return;
		}

		LearningReply reply;

		at::Tensor XVal(std::move(validationSet.first));
		at::Tensor YVal(std::move(validationSet.second));

		auto dataset = torch::data::datasets::TensorDataset(torch::stack({ XTrain, YTrue }, 1));
		auto dataloader = torch::data::make_data_loader(dataset, torch::data::DataLoaderOptions().batch_size(settings.batchSize));

		std::vector<at::Tensor> lossesOfDecade;

		auto start = high_resolution_clock::now();
		try
		{
			for (int epoch = 0; epoch < settings.epochsCount; epoch++)
			{
				std::vector<at::Tensor> lossesOfEpoch;
				reply.epochNumber = epoch;
				if (epoch != 0 && epoch % 10 == 0)
				{
					if (!lossesOfDecade.empty())
					{
						auto avg = std::accumulate(lossesOfDecade.begin(), lossesOfDecade.end(), torch::ones_like(*lossesOfDecade.begin())) / (double)lossesOfDecade.size();
						reply.message += QString("Decade loss: %1").arg(avg.item().toDouble());
						lossesOfDecade.clear();

						auto valPredicted = settings.nn->Predict(XVal);
						auto valLoss = settings.criterion->forward(valPredicted, YVal);
						reply.valLoss = valLoss.item().toDouble();

						DecadeFinished(reply);
					}
				}

				for (const auto& batch : *dataloader)
				{
					optimizer->zero_grad();

					for (const auto& sample : batch)
					{
						auto x = sample.data[0].reshape({ -1, 1 });
						auto yReal = sample.data[1].reshape({ -1, 1 });

						auto yPred = settings.nn->Predict(x);
						auto loss = settings.criterion->forward(yPred, yReal);
						lossesOfDecade.push_back(loss);
						loss.backward();
						lossesOfEpoch.push_back(loss);

						optimizer->step();
					}
				}

				reply.avgLoss = std::accumulate(lossesOfEpoch.begin(), lossesOfEpoch.end(), 0.0, [&](double left, const at::Tensor& right)
					{
						return left + right.item().toDouble();
					}) / lossesOfEpoch.size();

					EpochFinished(reply);
					stats.epochLosses.push_back(reply.avgLoss);
					reply.avgLoss = 0.0;
					reply.message.clear();
			}
		}
		catch (std::exception& e)
		{
			ErrorRaised(e.what());
		}

		auto end = high_resolution_clock::now();
		auto time = duration_cast<milliseconds>(end - start);
		stats.executionTime = time;
	}

	LearningStatistics NNCouch::GetStatistics() const noexcept
	{
		return stats;
	}

	bool NNCouch::isReady() const
	{
		LearningReply reply;
		if (!settings.nn)
		{
			ErrorRaised(QString::fromLocal8Bit("Нет модели для обучения!"));
			return false;
		}

		if (!optimizer)
		{
			ErrorRaised(QString::fromLocal8Bit("Оптимайзер не задан!"));
			return false;
		}

		if (!settings.criterion)
		{
			ErrorRaised(QString::fromLocal8Bit("Лосс-функция не задана!"));
			return false;
		}

		if (settings.batchSize <= 0)
		{
			ErrorRaised(QString::fromLocal8Bit("Значение размера батча должно быть положительным числом!"));
			return false;
		}

		if (settings.learningRate <= 0.0)
		{
			ErrorRaised(QString::fromLocal8Bit("Скорость обучения должна быть больше нуля!"));
			return false;
		}

		return true;
	}
}