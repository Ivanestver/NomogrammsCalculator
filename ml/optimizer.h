#pragma once

#include <memory>
#include "ml/includes.h"

namespace ml
{
	enum class OptimizerType
	{
		Adam
	};
	
	class OptimizerInitializer
	{
	public:
		static std::unique_ptr<OptimizerInitializer> CreateInitializer(OptimizerType optimizerType);

	public:
		OptimizerInitializer() = default;
		virtual ~OptimizerInitializer() = default;

		virtual std::shared_ptr<torch::optim::Optimizer> CreateOptimizer(double learningRate, const torch::nn::Module& module) const = 0;
	};

	class AdamInitializer : public OptimizerInitializer
	{
	public:
		AdamInitializer() = default;
		~AdamInitializer() override = default;

		std::shared_ptr<torch::optim::Optimizer> CreateOptimizer(double learningRate, const torch::nn::Module& module) const override;
	};
}