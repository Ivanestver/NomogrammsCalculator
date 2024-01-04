#include "nomogramms/NNWrapper.h"
#include "ml/NNStorage.h"
#include <vector>

namespace nomogramms
{
	NNWrapper::NNWrapper(std::map<ParameterType, std::vector<SMeasureUnit>>& ParametersList)
		: m_parametersList(ParametersList)
	{
	}
	bool NNWrapper::LoadNN(const QUuid& NNId, QString& error) noexcept
	{
		if (NNId.isNull())
		{
			error = QString::fromLocal8Bit("Пустой идентификатор. Не удалось загрузить нейронную сеть");
			return false;
		}

		const auto& storage = ml::NNStorage::GetInstance();
		m_nn = storage.GetNet(NNId);
		if (!m_nn)
		{
			error = QString::fromLocal8Bit("Не удалось загрузить нейронную сеть");
			return false;
		}

		return true;
	}

	bool NNWrapper::Calc(const IOData& inputData, IOData& outputData, QString& error) const noexcept
	{
		std::vector<double> inputValues;
		const auto itInput = m_parametersList.find(ParameterType::Input);
		if (itInput == m_parametersList.end())
		{
			error = QString::fromLocal8Bit("Во входных данных нет данных о входных параметрах");
			return false;
		}

		const auto itOutput = m_parametersList.find(ParameterType::Output);
		if (itOutput == m_parametersList.end())
		{
			error = QString::fromLocal8Bit("Во входных данных нет информации о выходных параметрах");
			return false;
		}

		for (const auto& measureUnit : itInput->second)
		{
			double value = inputData.GetValue(measureUnit);
			inputValues.push_back(value);
		}

		at::Tensor inputTensor = torch::zeros({1ULL, (long long)inputValues.size()});
		for (size_t row = 0; row < inputValues.size(); ++row)
		{
			inputTensor[0][row] = inputValues[row];
		}

		at::Tensor outputTensor;
		try
		{
			outputTensor = m_nn->Predict(inputTensor);
		}
		catch (const std::exception& e)
		{
			error = QString::fromLocal8Bit(e.what());
			return false;
		}

		size_t outputTensorSize = outputTensor.size(0);
		size_t outputParametersListSize = itOutput->second.size();
		size_t outputSize = std::min(outputTensorSize, outputParametersListSize);
		try
		{
			for (size_t i = 0; i < outputSize; ++i)
			{
				const auto& parameterType = itOutput->second[i];
				double calculatedValue = outputTensor[i][0].item<double>();
				outputData.AddValue(parameterType, calculatedValue);
			}	
		}
		catch (const std::exception& e)
		{
			error = e.what();
			return false;
		}

		return true;
	}
}