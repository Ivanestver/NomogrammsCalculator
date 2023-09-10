#pragma once
#include "DBObject.h"
#include "common/macros.h"

namespace nomogramms
{
	DECL_SHARED(Measure);
	#define MeasureClass_ID "F35C1C49-159C-4179-A092-B18980034414"

	class Measure : public DBObject
	{
		DECL_DBCLASS(Measure)
	public:
		Measure(const QUuid& id);
		~Measure() override = default;

		DBObject& operator=(const DBObject& other) override;

	private:
		void initFromDB() override;
	};
}