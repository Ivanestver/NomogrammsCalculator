#include "common/Unit.h"
#include "db/DataBaseWrapper.h"

namespace nomogramms
{
	Unit::Unit(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	DBObject& Unit::operator=(const DBObject& other)
	{
		return base::operator=(other);
	}

	void Unit::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		QString queryStr = "select [unit_name] from [unit] where [unit_id] = ?";
		QString error;
		auto response = db->ExecuteQuery(queryStr, { GetId() }, error);
		if (response.empty() || response[0].empty())
			return;
		setName(response.front().front().toString());
	}
}