#include "nomogramms/BypassRule.h"

namespace nomogramms
{
	BypassRule::BypassRule(const QUuid& id)
		: base(id)
	{}

	BypassRule::BypassRule(const BypassRule& other)
		: base(other)
	{
		graphics.clear();
		this->graphics = other.graphics;
	}

	BypassRule::~BypassRule() = default;

	bool BypassRule::operator==(const DBObject& other)
	{
		if (*this == other)
			return true;

		const auto& o = static_cast<const BypassRule&>(other);

		return base::operator==(other)
			&& this->graphics == o.graphics;
	}

	bool BypassRule::operator!=(const DBObject& other)
	{
		return !operator==(other);
	}

	BypassRule& BypassRule::operator=(const DBObject& other)
	{
		if (*this == other)
			return *this;

		base::operator=(other);

		const auto& o = static_cast<const BypassRule&>(other);
		this->graphics = o.graphics;

		return *this;
	}

	bool BypassRule::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
	{
		error = "Method 'Calculate' is not implemented in BypassRule class";
		return false;
	}

	void BypassRule::GetParameters(std::map<ParameterType, std::vector<SMeasureUnit>>& parameters) const
	{
		for (const auto& g : graphics)
			g->GetParameters(parameters);
	}

	void BypassRule::initFromDB()
	{}
}