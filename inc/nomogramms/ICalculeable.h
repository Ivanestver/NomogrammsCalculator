#pragma once
#include "nomogramms/IOData.h"
#include <QString>
#include <vector>
#include "common/macros.h"
#include "nomogramms/NomogrammsEnums.h"
#include "nomogramms/MeasureUnit.h"
#include <set>

namespace nomogramms
{
	DECL_SHARED(ICalculeable);

	class ICalculeable
	{
	public:
		struct SMeasureUnitLess
		{
		public:
			bool operator()(const SMeasureUnit& left, const SMeasureUnit& right) const
			{
				return *left < *right;
			}
		};

		using ParametersDict = std::map<ParameterType, std::set<SMeasureUnit, SMeasureUnitLess>>;
	public:
		virtual ~ICalculeable() = default;
		ICalculeable(const ICalculeable&) = default;
		ICalculeable(ICalculeable&&) noexcept = default;
		ICalculeable& operator=(const ICalculeable&) = default;
		ICalculeable& operator=(ICalculeable&&) noexcept = default;

		virtual bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const = 0;
		virtual void GetParameters(ParametersDict& parameters) const = 0;

	//protected:
		ICalculeable() = default;
	};
}