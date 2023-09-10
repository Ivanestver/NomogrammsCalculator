#pragma once

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include <QString>
#include <vector>

namespace ml
{
	class FullyConnectedNN : public torch::nn::Module
	{
	public:
		FullyConnectedNN(int inputParamsCount, const std::vector<int>& layers, int outputParamsCount, const QString& nnName = QString::fromLocal8Bit("NN"));
		~FullyConnectedNN() override = default;

		virtual at::Tensor Predict(const at::Tensor& X);

	private:
		std::vector<torch::nn::Linear> linearLayers;
		torch::nn::Linear lastLinearLayer{ nullptr };
	};
}