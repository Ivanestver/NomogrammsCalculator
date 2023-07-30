#include "common/MeasureUnit.h"
#include "db/DataBaseWrapper.h"

namespace nomogramms
{
	MeasureUnit::MeasureUnit(const QUuid& id)
		: base(id)
	{}

	MeasureUnit::MeasureUnit(const MeasureUnit& other)
		: base(other)
	{}

	MeasureUnit::~MeasureUnit() = default;

	bool MeasureUnit::operator==(const DBObject& other)
	{
		return base::operator==(other);
	}

	bool MeasureUnit::operator!=(const DBObject& other)
	{
		return base::operator!=(other);
	}

	DBObject& MeasureUnit::operator=(const DBObject& other)
	{
		return base::operator=(other);
	}

	void MeasureUnit::initFromDB()
	{}
}