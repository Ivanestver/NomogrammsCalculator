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

		QString queryString = "select class_id, sub_id, extra from ([template] as t1 inner join [template_template] as t2 on t1.template_id=t2.sub_id) where [master_id] = ?";
		std::vector<QVariant> params { GetId() };
		QString error;

		auto result = db->ExecuteQuery(queryString, params, error);
		if (!error.isEmpty())
			return;

		for (const auto& v : result)
		{
			QUuid classId = v[0].value<QUuid>();
			QUuid itemId = v[1].value<QUuid>();
			auto g = getICalculeableByCID(classId, itemId);

			QString label = v[2].value<QString>();
			graphics.insert({ label, g });
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