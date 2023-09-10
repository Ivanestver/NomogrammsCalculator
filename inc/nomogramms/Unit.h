#pragma once
#include "DBObject.h"
#include <QUuid>
#include "common/macros.h"

namespace nomogramms
{
	DECL_SHARED(Unit);
#define UnitClass_ID "6E671449-E801-44B2-9C87-0C093C27E630"

	class Unit : public DBObject
	{
		DECL_DBCLASS(Unit);

	public:
		Unit(const QUuid& id);
		~Unit() override = default;

		DBObject& operator=(const DBObject& other) override;

	private:
		void initFromDB() override;
	};
}