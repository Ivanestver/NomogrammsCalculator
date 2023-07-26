#pragma once
#include <QObject>
#include <QUuid>
#include <map>
#include <vector>
#include "common/NomogrammsEnums.h"
#include "common/Measure.h"
#include "common/macros.h"
#include "common/DBObject.h"
#include "nomogramms/ICalculeable.h"
#include "common/IOData.h"

namespace nomogramms
{
	DECL_SHARED(Graphics);

	#define GraphicsClass_ID "6DCE13AE-8897-48EC-9B4E-664845D40D73"
	class Graphics : public DBObject, public ICalculeable
	{
		DECL_DBCLASS(Graphics);
	public:
		Graphics(const QUuid& id);
		Graphics(const Graphics& other);

		const std::vector<SMeasure> GetParametersByType(ParameterType type) const;

		bool operator==(const DBObject& other) override;
		bool operator!=(const DBObject& other) override;
		Graphics& operator=(const DBObject& other) override;

		bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const override;
		void GetParameters(std::map<ParameterType, std::vector<SMeasure>>& parameters) const override;

	protected:
		virtual void initFromDB() override;

	private:
		std::map<ParameterType, std::vector<SMeasure>> parametersList;
		void* neuralNetwork;
	};
}