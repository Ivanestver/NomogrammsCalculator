#include "optimizer.h"

namespace ml
{
    std::unique_ptr<OptimizerInitializer> OptimizerInitializer::CreateInitializer(OptimizerType optimizerType)
    {
        if (optimizerType == OptimizerType::Adam)
            return std::make_unique<AdamInitializer>();
        return std::unique_ptr<OptimizerInitializer>();
    }

    std::shared_ptr<torch::optim::Optimizer> AdamInitializer::CreateOptimizer(double learningRate, const torch::nn::Module& module) const
    {
        return std::make_shared<torch::optim::Adam>(module.parameters(), torch::optim::AdamOptions(learningRate));
    }
}
