#pragma once
#include "DBObject.h"
#include "MeasureUnit.h"
#include "macros.h"

namespace nomogramms
{
	DECL_SHARED(Measure);
	#define MeasureClass_ID "F35C1C49-159C-4179-A092-B18980034414"

	class Measure : public DBObject
	{
		DECL_DBCLASS(Measure)
	public:
		Measure(const QUuid& id);
		Measure(const Measure& other);
		~Measure() override;

		const SMeasureUnit GetMeasureUnit() const;

		void SetValue(double value);
		double GetValue() const;

		bool operator==(const DBObject& other) override;
		bool operator!=(const DBObject& other) override;
		Measure& operator=(const DBObject& other) override;

	protected:
		virtual void initFromDB() override;

	private:
		SMeasureUnit measureUnit;
		double value;
	};
}