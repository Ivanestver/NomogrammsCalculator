#pragma once
#include "DBObject.h"
#include "common/macros.h"
#include "QUuid"
#include "Unit.h"
#include "Measure.h"

namespace nomogramms
{
	DECL_SHARED(MeasureUnit);
	#define MeasureUnitClass_ID "9BBF6C28-9C01-49B2-ACA2-104699F5ECF4"

	class MeasureUnit : public DBObject
	{
		DECL_DBCLASS(MeasureUnit)
	public:
		MeasureUnit(const QUuid& id);

		bool operator==(const DBObject& other) const override;
		bool operator!=(const DBObject& other) const override;


	protected:
		virtual void initFromDB() override;

	private:
		SMeasure measure = nullptr;
		SUnit unit = nullptr;
	};

	bool operator<(const MeasureUnit& left, const MeasureUnit& right);
}