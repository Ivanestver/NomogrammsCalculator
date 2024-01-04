#include "nomogramms/BypassRule.h"

namespace nomogramms
{
	BypassRule::BypassRule(const QUuid& id)
		: base(id)
	{}

	bool BypassRule::operator==(const DBObject& other) const
	{
		if (*this == other)
			return true;

		const auto& o = static_cast<const BypassRule&>(other);

		return base::operator==(other)
			&& this->graphics == o.graphics;
	}

	bool BypassRule::operator!=(const DBObject& other) const
	{
		return !operator==(other);
	}

	bool BypassRule::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
	{
		Q_UNUSED(inputData);
		Q_UNUSED(outputData);
		Q_UNUSED(error);
		error = "Method 'Calculate' is not implemented in BypassRule class";
		return false;
	}

	void BypassRule::GetParameters(ICalculeable::ParametersDict& parameters) const
	{
		for (const auto& g : graphics)
			g->GetParameters(parameters);
	}

	void BypassRule::initFromDB()
	{}
}