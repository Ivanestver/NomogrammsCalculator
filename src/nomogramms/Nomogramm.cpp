#include "nomogramms/Nomogramm.h"
#include <QVariant>
#include "db/DataBaseWrapper.h"
#include "db_state/properties.h"
#include "nomogramms/ExpressionExecutor.h"

namespace nomogramms
{
	Nomogramm::Nomogramm(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	Nomogramm::Nomogramm(const Nomogramm& other)
		: base(other)
	{}

	bool Nomogramm::operator==(const DBObject& other)
	{
		const auto& o = static_cast<const Nomogramm&>(other);

		return base::operator==(other);
	}

	bool Nomogramm::operator!=(const DBObject& other)
	{
		return !operator==(other);
	}

	Nomogramm& Nomogramm::operator=(const DBObject& other)
	{
		if (*this == other)
			return *this;

		base::operator=(other);
		const auto& o = static_cast<const Nomogramm&>(other);

		return *this;
	}

	bool Nomogramm::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
	{
		ExpressionExecutor executor(bypassRule, &inputData);
		for (const auto& pair : graphics)
			executor.SetVariable(pair.first, pair.second);

		if (!executor.Exec())
		{
			error = executor.GetError();
			return false;
		}

		executor.WriteResultsTo(outputData);

		return true;
	}

	void Nomogramm::GetParameters(std::map<ParameterType, std::vector<SMeasure>>& parameters) const
	{
		for (const auto& pair : graphics)
			pair.second->GetParameters(parameters);
	}

	void Nomogramm::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		bypassRule = db->GetPropertyValueByIdAndTemplateID(db_state::properties::nomogramm_bypassRule_name, GetId());

		QString queryString = "select slave_id, master_slave_value from template_template where master_id = :1";
		std::vector<QVariant> params { QVariant(GetId()) };
		QString error;

		auto result = db->ExecuteQuery(queryString, params, error);
		if (!error.isEmpty())
			return;

		for (const auto& v : result)
		{
			QString label = v[1].value<QString>();
			auto g = std::make_shared<Graphics>(v[0].value<QUuid>());
			graphics.insert({ label, g });
		}
	}
}