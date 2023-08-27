#pragma once
#include "common/DBObject.h"
#include "common/macros.h"
#include "Graphics.h"
#include "BypassRule.h"
#include "ICalculeable.h"
#include <QString>

namespace nomogramms
{
	DECL_SHARED(Nomogramm);

	#define NomogrammClass_ID "F5313633-C8FC-43DC-A92E-88B7EE8DF439"
	class Nomogramm : public DBObject, public ICalculeable
	{
		DECL_DBCLASS(Nomogramm);

	public:
		Nomogramm(const QUuid& id);
		Nomogramm(const Nomogramm& other);

		bool operator==(const DBObject& other) override;
		bool operator!=(const DBObject& other) override;
		Nomogramm& operator=(const DBObject& other) override;

		bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const override;
		void GetParameters(std::map<ParameterType, std::vector<SMeasure>>& parameters) const override;
		bool GetChildren(std::vector<SDBObject>& children) const override;

	protected:
		virtual void initFromDB() override;

	private:
		std::map<QString, SICalculeable> graphics;
		QString bypassRule;
	};
}