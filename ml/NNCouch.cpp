#include "NNCouch.h"
#include "ml/optimizer.h"

namespace ml
{
	NNCouch::NNCouch(const NNCouchSettings& settings_)
		: settings(settings_)
	{
		auto optimizerInitializer = OptimizerInitializer::CreateInitializer(settings.optimizerType);
		optimizer = optimizerInitializer->CreateOptimizer(settings.learningRate, *settings.nn);
	}

	void NNCouch::Train(const at::Tensor& XTrain, const at::Tensor& YTrue)
	{
		LearningReply reply;

		if (!isReady())
			return;

		if (XTrain.sizes().empty() || YTrue.sizes().empty())
		{
			ErrorRaised(QString::fromLocal8Bit("Входные данные не валидны"));
			return;
		}

		auto dataset = torch::data::datasets::TensorDataset(torch::stack({ XTrain, YTrue }, 1));
		auto dataloader = torch::data::make_data_loader(dataset, torch::data::DataLoaderOptions().batch_size(settings.batchSize));

		std::vector<at::Tensor> lossesOfDecade;

		try
		{
			for (int epoch = 0; epoch < settings.epochsCount; epoch++)
			{
				std::vector<at::Tensor> lossesOfEpoch;
				reply.epochNumber = epoch;
				if (epoch % (settings.epochsCount / 10) == 0)
				{
					reply.message = QString("Epoch #%1").arg(epoch);
					if (!lossesOfDecade.empty())
					{
						reply.message += QString("\nDecade loss: %1").arg((std::accumulate(lossesOfDecade.begin(), lossesOfDecade.end(), torch::ones_like(*lossesOfDecade.begin())) / (double)lossesOfDecade.size()).item().toDouble());
						lossesOfDecade.clear();
					}
					DecadeFinished(reply);
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
					reply.avgLoss = 0.0;
					reply.message.clear();
			}
		}
		catch (std::exception& e)
		{
			ErrorRaised(e.what());
			return;
		}
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