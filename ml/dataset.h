#pragma once

#include <torch/torch.h>

namespace ml
{
	class DataSet : public torch::data::Dataset<DataSet>
	{
	public:
		explicit DataSet(const at::Tensor& X_features, const at::Tensor& Y_labels);

		// Унаследовано через Dataset
		torch::data::Example<> get(size_t index) override;
		c10::optional<size_t> size() const override;

	private:
		at::Tensor xFeatures, yFeatures;

	};
}