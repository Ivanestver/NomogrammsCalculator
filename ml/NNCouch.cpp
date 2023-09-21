#include "NNCouch.h"

namespace ml
{
	NNCouch::NNCouch(const NNCouchSettings& settings_)
		: settings(settings_)
	{}

	void NNCouch::Train(const at::Tensor& XTrain, const at::Tensor& YTrue, const OnMessageRepliedListener& listener)
	{
		if (!listener)
			return;

		if (!isReady(listener))
			return;

		if (XTrain.sizes().empty() || YTrue.sizes().empty())
		{
			listener(QString::fromLocal8Bit("Входные данные не валидны"));
			return;
		}

		auto dataset = torch::data::datasets::TensorDataset(torch::stack({ XTrain, YTrue }, 1));
		auto dataloader = torch::data::make_data_loader(dataset, torch::data::DataLoaderOptions().batch_size(settings.batchSize));

		std::vector<at::Tensor> lossesOfEpoch;

		for (int epoch = 0; epoch < settings.epochsCount; epoch++)
		{
			if (epoch % (settings.epochsCount / 10) == 0)
			{
				QString message = QString("Epoch #%1").arg(epoch);
				listener(message);
				if (!lossesOfEpoch.empty())
				{
					auto message = QString("Decade loss: %1").arg((std::accumulate(lossesOfEpoch.begin(), lossesOfEpoch.end(), torch::ones_like(*lossesOfEpoch.begin())) / (double)lossesOfEpoch.size()).item().toDouble());
					listener(message);
					lossesOfEpoch.clear();
				}
			}

			for (const auto& batch : *dataloader)
			{
				settings.optimizer->zero_grad();

				for (const auto& sample : batch)
				{
					auto x = sample.data[0].reshape({ -1, 1 });
					auto yReal = sample.data[1].reshape({ -1, 1 });

					auto yPred = settings.nn->Predict(x);
					auto loss = (*settings.criterion)(yPred, yReal);
					lossesOfEpoch.push_back(loss);
					loss.backward();

					settings.optimizer->step();
				}
			}
		}
	}

	bool NNCouch::isReady(const OnMessageRepliedListener& listener) const
	{
		if (!settings.nn)
		{
			listener(QString::fromLocal8Bit("Нет модели для обучения!"));
			return false;
		}

		if (!settings.optimizer)
		{
			listener(QString::fromLocal8Bit("Оптимайзер не задан!"));
			return false;
		}

		if (!settings.criterion)
		{
			listener(QString::fromLocal8Bit("Лосс-функция не задана!"));
			return false;
		}

		if (settings.batchSize <= 0)
		{
			listener(QString::fromLocal8Bit("Значение размера батча должно быть положительным числом!"));
			return false;
		}

		if (settings.learningRate <= 0.0)
		{
			listener(QString::fromLocal8Bit("Скорость обучения должна быть больше нуля!"));
			return false;
		}

		return true;
	}
}