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

	void MeasureUnit::initFromDB()
	{}
}