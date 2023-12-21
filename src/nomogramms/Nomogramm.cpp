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

	bool Nomogramm::operator==(const DBObject& other) const
	{
		const auto& o = static_cast<const Nomogramm&>(other);

		return base::operator==(other)
			&& this->bypassRule == o.bypassRule;
	}

	bool Nomogramm::operator!=(const DBObject& other) const
	{
		return !operator==(other);
	}

	bool Nomogramm::Calculate(const IOData& inputData, IOData& outputData, QString& error) const
	{
		ExpressionExecutor executor(bypassRule, &inputData);
		for (const auto& pair : graphics)
		{
			if (!executor.SetVariable(pair.first, pair.second, error))
				return false;
		}

		if (!executor.Exec())
		{
			error = executor.GetError();
			return false;
		}

		executor.WriteResultsTo(outputData);

		//Nomogramm n1{ QUuid{} };
		//Nomogramm n2 = n1;
		//Nomogramm n3 = std::move(n1);

		return true;
	}

	void Nomogramm::GetParameters(std::map<ParameterType, std::vector<SMeasureUnit>>& parameters) const
	{
		for (const auto& pair : graphics)
			pair.second->GetParameters(parameters);
	}

	bool Nomogramm::GetChildren(std::vector<SDBObject>& children) const
	{
		for (const auto& pair : graphics)
		{
			children.push_back(std::dynamic_pointer_cast<DBObject>(pair.second));
		}
		return true;
	}

	void Nomogramm::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		QString error;
		bypassRule = db->GetPropertyValueByIdAndTemplateID(db_state::properties::nomogramm_bypassRule, GetId(), error);

		QString queryString = "select class_id, sub_id, extra from (template as t1 inner join template_template as t2 on t1.template_id=t2.sub_id) where master_id = ?";
		std::vector<QVariant> params { GetId() };

		auto result = db->ExecuteQuery(queryString, params, error);
		if (result.empty())
			return;

		for (const auto& v : result)
		{
			QUuid classId = v[0].value<QUuid>();
			QUuid itemId = v[1].value<QUuid>();
			auto g = getICalculeableByCID(classId, itemId);

			QString label = v[2].value<QString>();
			graphics.insert({ label, g });
		}

		queryString = "select measure_unit_id from template_measure_unit_input where template_id = ?";
		result = db->ExecuteQuery(queryString, params, error);
		if (result.empty() || result[0].empty())
			return;

		for (const auto& id_ : result[0])
		{
			auto measureUnit = std::make_shared<MeasureUnit>(id_.toUuid());
			parameters.push_back(measureUnit);
		}

	}

	std::shared_ptr<ICalculeable> Nomogramm::getICalculeableByCID(const QUuid& classId, const QUuid& itemId) const
	{
		if (classId == Nomogramm::GetCID())
			return std::make_shared<Nomogramm>(itemId);
		else
			return std::make_shared<Graphics>(itemId);
	}
}