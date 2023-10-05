#include "AbstractNN.h"

namespace ml
{
	FullyConnectedNN::FullyConnectedNN(int inputParamsCount, const std::vector<int>& layers, int outputParamsCount, const QString& nnName)
		:torch::nn::Module(nnName.toStdString())
	{
		if (layers.empty())
		{
			lastLinearLayer = register_module("last_linear_layer", torch::nn::Linear(inputParamsCount, outputParamsCount));
		}
		else
		{
			auto inputLayer = register_module("input_layer", torch::nn::Linear(inputParamsCount, layers.empty() ? outputParamsCount : *layers.begin()));
			linearLayers.push_back(inputLayer);

			for (size_t i = 1; i < layers.size(); i++)
			{
				QString layerName = QString("linear_layer %1").arg(i);
				auto layer = register_module("input_layer", torch::nn::Linear(inputParamsCount, layers.empty() ? outputParamsCount : *layers.begin()));
			}

			lastLinearLayer = register_module("last_linear_layer", torch::nn::Linear(*layers.rbegin(), outputParamsCount));
		}
	}

	at::Tensor FullyConnectedNN::Predict(const at::Tensor& X)
	{
		at::Tensor Y(X);
		for (auto& linearLayer : linearLayers)
		{
			Y = linearLayer(Y);
			Y = torch::relu(Y);
		}

		return lastLinearLayer(Y);
	}

	void FullyConnectedNN::Print(QStringList& modules) const
	{
		QList<QString> list;
		for (const auto& layer : linearLayers)
		{
			list.append(layer->name().c_str());
			list.append("ReLU");
		}
		
		if (!list.isEmpty())
			list.pop_back();

		list.append(lastLinearLayer->name().c_str());

		modules.append(list);
	}
}