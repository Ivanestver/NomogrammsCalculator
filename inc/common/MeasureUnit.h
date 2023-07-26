#pragma once
#include "DBObject.h"
#include "macros.h"
#include "QUuid"

namespace nomogramms
{
	DECL_SHARED(MeasureUnit);
	#define MeasureUnitClass_ID "9BBF6C28-9C01-49B2-ACA2-104699F5ECF4"

	class MeasureUnit : public DBObject
	{
		DECL_DBCLASS(MeasureUnit)
	public:
		MeasureUnit(const QUuid& id);
		MeasureUnit(const MeasureUnit& other);
		~MeasureUnit() override;

	protected:
		virtual void initFromDB() override;
	};
}