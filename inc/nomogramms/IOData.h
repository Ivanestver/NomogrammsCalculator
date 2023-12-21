#pragma once
#include <map>
#include "MeasureUnit.h"

namespace nomogramms
{
	class IOData
	{
	public:
		IOData() = default;
		IOData(const IOData& other);
		IOData(IOData&& other) noexcept;

		void AddValue(const SMeasureUnit& measure, double value);
		double GetValue(const SMeasureUnit& measure) const;

		IOData& operator=(const IOData& other);
		IOData& operator=(IOData&& other) noexcept;

		IOData& operator+=(const IOData& other);

		bool operator==(const IOData& other);
		bool operator!=(const IOData& other);

		void Clear();
		bool IsEmpty() const noexcept;

	private:
		std::map<SMeasureUnit, double> m_data;
	};
}