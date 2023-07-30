#include "common/Measure.h"
#include "db/DataBaseWrapper.h"
#include <QVariant>
#include "db/properties.h"

namespace nomogramms
{
	Measure::Measure(const QUuid& id)
		: base(id)
		, value(0.0)
	{
	}

	Measure::Measure(const Measure& other)
		: base(other)
	{
		measureUnit = other.measureUnit;
		this->value = other.value;
	}

	Measure::~Measure() = default;

	const SMeasureUnit Measure::GetMeasureUnit() const
	{
		return measureUnit;
	}

	void Measure::SetValue(double value_)
	{
		this->value = value_;
	}

	double Measure::GetValue() const
	{
		return value;
	}

	bool Measure::operator==(const DBObject& other)
	{
		const auto& o = static_cast<const Measure&>(other);
		return base::operator==(other)
			&& this->measureUnit->operator==(*o.measureUnit.get())
			&& this->value == o.value;
	}

	bool Measure::operator!=(const DBObject& other)
	{
		return !operator==(other);
	}

	Measure& Measure::operator=(const DBObject& other)
	{
		const auto& o = static_cast<const Measure&>(other);
		if (*this == o)
			return *this;

		base::operator=(other);
		measureUnit->operator=(*o.measureUnit.get());

		return *this;
	}

	void Measure::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		value = db->GetAttributeByIdAndTemplateID(db::properties::measure_value, GetId()).toDouble();

		QString queryString = "select slave_id from template_template where master_id = :1";
		std::vector<QVariant> params{QVariant(GetId())};
		QString error;
		auto result = db->ExecuteQuery(queryString, params, error);
		if (!error.isEmpty())
		{
			return;
		}

		for (const auto& v : result)
			measureUnit = std::make_shared<MeasureUnit>(v.at(0).value<QUuid>());
	}
}