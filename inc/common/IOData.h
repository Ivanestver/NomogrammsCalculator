#pragma once
#include <map>
#include "Measure.h"

namespace nomogramms
{
	class IOData
	{
	public:
		IOData() = default;
		IOData(const IOData& other);

		void AddValue(const nomogramms::SMeasure& measure, double value);
		double GetValue(const nomogramms::SMeasure& measure);

		IOData& operator=(const IOData& other);
		IOData& operator+=(const IOData& other);

		bool operator==(const IOData& other);
		bool operator!=(const IOData& other);

		void Clear();

	private:
		std::map<nomogramms::SMeasure, double> data;
	};
}