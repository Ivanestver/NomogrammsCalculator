#include "ml/dataset.h"

namespace ml
{
	DataSet::DataSet(const at::Tensor& X_features, const at::Tensor& Y_labels)
		: xFeatures(std::move(X_features))
		, yFeatures(std::move(Y_labels))
	{
	}

	torch::data::Example<> DataSet::get(size_t index)
	{
		return { xFeatures[index], yFeatures[index] };
	}

	c10::optional<size_t> DataSet::size() const
	{
		return std::min(xFeatures.size(0), yFeatures.size(0));
	}
}