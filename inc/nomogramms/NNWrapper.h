#pragma once

#include "ml/AbstractNN.h"
#include <QUuid>
#include <QString>
#include "nomogramms/IOData.h"
#include <map>
#include "nomogramms/NomogrammsEnums.h"


namespace nomogramms
{
	class NNWrapper
	{
	public:
		NNWrapper(std::map<ParameterType, std::vector<SMeasureUnit>>& ParametersList);
		bool LoadNN(const QUuid& NNId, QString& error) noexcept;
		bool Calc(const IOData& inputData, IOData& outputData, QString& error) const noexcept;

	private:
		std::map<ParameterType, std::vector<SMeasureUnit>>& m_parametersList;
		ml::SFullyConnectedNN m_nn;
	};
}