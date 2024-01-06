#include "nomogramms/IOData.h"

namespace nomogramms
{
	IOData::IOData(const IOData& other)
	{
		this->m_data = other.m_data;
	}

	IOData::IOData(IOData&& other) noexcept
		: m_data(std::move(other.m_data))
	{
	}

	void IOData::AddValue(const nomogramms::SMeasureUnit& measure, double value)
	{
		auto it = m_data.find(measure);
		if (it != m_data.end())
			return;

		m_data.insert({ measure, value });
	}

	double IOData::GetValue(const nomogramms::SMeasureUnit& measure) const
	{
		const auto it = std::find_if(m_data.begin(), m_data.end(), [&measure](const std::pair<SMeasureUnit, double>& item)
			{
				return *item.first == *measure;
			});
		if (it == m_data.end())
			return 0.0;

		return it->second;
	}

	IOData& IOData::operator=(const IOData& other)
	{
		if (*this == other)
			return *this;

		this->m_data.clear();
		this->m_data.insert(other.m_data.begin(), other.m_data.end());

		return *this;
	}

	IOData& IOData::operator=(IOData&& other) noexcept
	{
		if (*this == other)
			return *this;

		this->m_data.clear();
		this->m_data = std::move(other.m_data);

		return *this;
	}

	IOData& IOData::operator+=(const IOData& other)
	{
		if (*this == other)
			return *this;

		m_data.insert(other.m_data.begin(), other.m_data.end());

		return *this;
	}

	bool IOData::operator==(const IOData& other)
	{
		return m_data.size() == other.m_data.size()
			&& std::equal(m_data.begin(), m_data.end(), other.m_data.begin(), other.m_data.end());
	}

	bool IOData::operator!=(const IOData& other)
	{
		return !operator==(other);
	}

	void IOData::Clear()
	{
		m_data.clear();
	}

	bool IOData::IsEmpty() const noexcept
	{
		return m_data.empty();
	}
}