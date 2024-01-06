#pragma once
#include <QObject>
#include <QUuid>
#include <map>
#include <vector>
#include <map>
#include "nomogramms/NomogrammsEnums.h"
#include "nomogramms/Measure.h"
#include "common/macros.h"
#include "nomogramms/DBObject.h"
#include "nomogramms/IOData.h"
#include "nomogramms/ICalculeable.h"
#include "ml/AbstractNN.h"
#include "NNWrapper.h"

namespace nomogramms
{
	DECL_SHARED(Graphics);

	#define GraphicsClass_ID "6DCE13AE-8897-48EC-9B4E-664845D40D73"

	class Graphics : public DBObject, public ICalculeable
	{
		DECL_DBCLASS(Graphics);
	public:
		Graphics(const QUuid& id);

		const std::vector<SMeasureUnit>& GetParametersByType(ParameterType type) const;

		bool operator==(const DBObject& other) const override;
		bool operator!=(const DBObject& other) const override;

		bool Calculate(const IOData& inputData, IOData& outputData, QString& error) const override;
		void GetParameters(ICalculeable::ParametersDict& parameters) const override;

	protected:
		virtual void initFromDB() override;

	private:
		std::map<ParameterType, std::vector<SMeasureUnit>> m_parametersList;
		ml::SFullyConnectedNN m_network{ nullptr };
		NNWrapper m_wrapper;
	};

}