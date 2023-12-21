#include "nomogramms/MeasureUnit.h"
#include "db/DataBaseWrapper.h"

namespace nomogramms
{
	MeasureUnit::MeasureUnit(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	bool MeasureUnit::operator==(const DBObject& other) const
	{
		return base::operator==(other);
	}

	bool MeasureUnit::operator!=(const DBObject& other) const
	{
		return base::operator!=(other);
	}

	void MeasureUnit::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		QString queryStr = "select measure_unit_name from measure_unit where measure_unit_id = ?";
		QString error;
		auto response = db->ExecuteQuery(queryStr, {GetId()}, error);
		if (response.empty() || response[0].empty())
			return;
		setName(response.front().front().toString());

		queryStr = "select measure_id, unit_id from measure_unit where measure_unit_id = ?";
		response = db->ExecuteQuery(queryStr, { GetId() }, error);
		if (response.empty() || response[0].empty())
			return;

		const auto& record = response[0];
		measure = std::make_shared<Measure>(record[0].toUuid());
		unit = std::make_shared<Unit>(record[1].toUuid());
	}
}