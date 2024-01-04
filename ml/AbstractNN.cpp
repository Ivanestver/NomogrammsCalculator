#include "AbstractNN.h"

namespace ml
{
	FullyConnectedNN::FullyConnectedNN(const QString& nnName)
		: torch::nn::Module(nnName.toStdString())
	{
	}
	
	FullyConnectedNN::FullyConnectedNN(int inputParamsCount, const std::vector<int>& layers, int outputParamsCount, const QString& nnName)
		: torch::nn::Module(nnName.toStdString())
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
				auto layer = register_module("input_layer", torch::nn::Linear(layers[i - 1], layers[i]));
			}

			lastLinearLayer = register_module("last_linear_layer", torch::nn::Linear(*layers.rbegin(), outputParamsCount));
		}
	}

	at::Tensor FullyConnectedNN::Predict(const at::Tensor& X) noexcept(false)
	{
		at::Tensor Y(X);
		for (auto& linearLayer : linearLayers)
		{
			Y = linearLayer(Y);
			Y = torch::relu(Y);
		}

		return lastLinearLayer(Y);
	}

	void FullyConnectedNN::Print(QStringList& modules) const noexcept
	{
		if (linearLayers.empty())
			return;

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

	std::pair<int, int> FullyConnectedNN::GetInputOutputParamsCount() const noexcept
	{
		auto modules = this->modules(false);
		int input{ 0 };
		int output{ 0 };
		if (!modules.empty())
		{
			const auto& first = modules.front();
			auto linearInput = std::dynamic_pointer_cast<torch::nn::LinearImpl>(first);
			input = linearInput->options.in_features();

			const auto& last = modules.back();
			linearInput = std::dynamic_pointer_cast<torch::nn::LinearImpl>(last);
			output = linearInput->options.out_features();
		}

		return { input, output };
	}

	int FullyConnectedNN::getParamsCountOfModule(const std::shared_ptr<torch::nn::Module>& module) const noexcept
	{
		if (!module)
			return 0;

		auto paramsOfLast = module->parameters(false);
		if (paramsOfLast.empty())
			return 0;

		return paramsOfLast[0].size(1);
	}
}