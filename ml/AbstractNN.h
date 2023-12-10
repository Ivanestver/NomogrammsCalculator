#pragma once

#include "ml/includes.h"
#include <QString>
#include <QStringList>
#include <vector>
#include "common/macros.h"

namespace ml
{
	DECL_SHARED(FullyConnectedNN);

	class FullyConnectedNN : public torch::nn::Module
	{
	public:
		FullyConnectedNN(int inputParamsCount, const std::vector<int>& layers, int outputParamsCount, const QString& nnName = QString::fromLocal8Bit("NN"));

		virtual at::Tensor Predict(const at::Tensor& X);

		void Print(QStringList& modules) const;

		std::pair<int, int> GetInputOutputParamsCount() const noexcept;

	private:
		int getParamsCountOfModule(const std::shared_ptr<torch::nn::Module>& module) const noexcept;

	private:
		std::vector<torch::nn::Linear> linearLayers;
		torch::nn::Linear lastLinearLayer{ nullptr };
	};
}