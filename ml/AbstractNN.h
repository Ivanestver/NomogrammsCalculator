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
		explicit FullyConnectedNN(const QString& nnName);
		FullyConnectedNN(int inputParamsCount, const std::vector<int>& layers, int outputParamsCount, const QString& nnName = QString::fromLocal8Bit("NN"));

		virtual at::Tensor Predict(const at::Tensor& X) noexcept(false);

		void Print(QStringList& modules) const noexcept;

		std::pair<int, int> GetInputOutputParamsCount() const noexcept;

	private:
		int getParamsCountOfModule(const std::shared_ptr<torch::nn::Module>& module) const noexcept;

	private:
		std::vector<torch::nn::Linear> linearLayers;
		torch::nn::Linear lastLinearLayer{ nullptr };
	};
}