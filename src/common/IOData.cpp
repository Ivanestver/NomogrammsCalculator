#include "common/IOData.h"

namespace nomogramms
{
	IOData::IOData(const IOData& other)
	{
		this->data = other.data;
	}

	IOData::IOData(IOData&& other) noexcept
		: data(std::move(other.data))
	{
	}

	void IOData::AddValue(const nomogramms::SMeasureUnit& measure, double value)
	{
		auto it = data.find(measure);
		if (it != data.end())
			return;

		data.insert({ measure, value });
	}

	double IOData::GetValue(const nomogramms::SMeasureUnit& measure) const
	{
		auto it = data.find(measure);
		if (it == data.end())
			return 0.0;

		return it->second;
	}

	IOData& IOData::operator=(const IOData& other)
	{
		if (*this == other)
			return *this;

		this->data.clear();
		this->data.insert(other.data.begin(), other.data.end());

		return *this;
	}

	IOData& IOData::operator=(IOData&& other) noexcept
	{
		if (*this == other)
			return *this;

		this->data.clear();
		this->data = std::move(other.data);

		return *this;
	}

	IOData& IOData::operator+=(const IOData& other)
	{
		if (*this == other)
			return *this;

		data.insert(other.data.begin(), other.data.end());

		return *this;
	}

	bool IOData::operator==(const IOData& other)
	{
		return data.size() == other.data.size()
			&& std::equal(data.begin(), data.end(), other.data.begin(), other.data.end());
	}

	bool IOData::operator!=(const IOData& other)
	{
		return !operator==(other);
	}

	void IOData::Clear()
	{
		data.clear();
	}
}