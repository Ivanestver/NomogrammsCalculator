#pragma once
#include "nomogramms/DBObject.h"
#include "Graphics.h"
#include <vector>
#include "common/macros.h"
#include "ICalculeable.h"
#include <nomogramms/IOData.h>
#include <nomogramms/NomogrammsEnums.h>

namespace nomogramms
{
	DECL_SHARED(BypassRule);

	#define BypassRuleClass_ID "BB75693A-4506-47DE-9DE6-6F40C8BC3C74"
	class BypassRule : public DBObject, public ICalculeable
	{
		DECL_DBCLASS(BypassRule);
	public:
		BypassRule(const QUuid& id);

		bool operator==(const DBObject& other) const override;
		bool operator!=(const DBObject& other) const override;

		virtual bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const override;
		virtual void GetParameters(ICalculeable::ParametersDict& parameters) const override;

	protected:
		virtual void initFromDB() override;

	private:
		std::vector<SICalculeable> graphics;
	};
}