#include "nomogramms/Measure.h"
#include "db/DataBaseWrapper.h"
#include <QVariant>
#include "db_state/properties.h"

namespace nomogramms
{
	Measure::Measure(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	DBObject& Measure::operator=(const DBObject& other)
	{
		return base::operator=(other);
	}

	void Measure::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		QString queryStr = "select [measure_name] from [measure] where [measure_id] = ?";
		QString error;
		auto response = db->ExecuteQuery(queryStr, { GetId() }, error);
		if (response.empty() || response[0].empty())
			return;
		setName(response.front().front().toString());
	}
}