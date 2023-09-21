#pragma once;

#include "ml/includes.h"

namespace ml
{
	class Criterion
	{
	public:
		virtual ~Criterion() = default;

		at::Tensor operator()(const at::Tensor& input, const at::Tensor& target);

		virtual at::Tensor forward(const at::Tensor& input, const at::Tensor& target) = 0;
		virtual void reset() = 0;
		virtual void pretty_print(std::ostream& stream) const = 0;
	};

	class MSECriterion : public Criterion
	{
	public:
		MSECriterion(const torch::nn::MSELoss& loss_);
		~MSECriterion() override = default;

		// Унаследовано через ICriterion
		virtual at::Tensor forward(const at::Tensor& input, const at::Tensor& target) override;
		virtual void reset() override;
		virtual void pretty_print(std::ostream& stream) const override;

	private:
		torch::nn::MSELoss loss{ nullptr };
	};
}