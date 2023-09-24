#include "criterion.h"

namespace ml
{
	MSECriterion::MSECriterion(const torch::nn::MSELoss& loss_)
		: loss(loss_)
	{
	}

	at::Tensor Criterion::operator()(const at::Tensor& input, const at::Tensor& target)
	{
		return forward(input, target);
	}

	at::Tensor MSECriterion::forward(const at::Tensor& input, const at::Tensor& target)
	{
		if (!loss)
			return at::Tensor();

		return loss->forward(input, target);
	}

	void MSECriterion::reset()
	{
		loss->reset();
	}

	void MSECriterion::pretty_print(std::ostream& stream) const
	{
		loss->pretty_print(stream);
	}
}