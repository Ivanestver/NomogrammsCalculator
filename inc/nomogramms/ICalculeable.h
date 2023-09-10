#pragma once
#include "nomogramms/IOData.h"
#include <QString>
#include <vector>
#include "common/macros.h"
#include "nomogramms/NomogrammsEnums.h"
#include "nomogramms/MeasureUnit.h"

namespace nomogramms
{
	DECL_SHARED(ICalculeable);

	class ICalculeable
	{
	public:
		using ParametersDict = std::map<ParameterType, std::vector<SMeasureUnit>>;
	public:
		virtual ~ICalculeable() = 0 {};
		virtual bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const = 0;
		virtual void GetParameters(ParametersDict& parameters) const = 0;

	protected:
		ICalculeable() = default;
	};
}