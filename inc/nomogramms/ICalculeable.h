#pragma once
#include "common/IOData.h"
#include <QString>
#include <vector>
#include "common/macros.h"
#include "common/NomogrammsEnums.h"

namespace nomogramms
{
	DECL_SHARED(ICalculeable);

	class ICalculeable
	{
	public:
		virtual ~ICalculeable() = 0 {};
		virtual bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const = 0;
		virtual void GetParameters(std::map<ParameterType, std::vector<SMeasure> >& parameters) const = 0;

	protected:
		ICalculeable() = default;
	};
}