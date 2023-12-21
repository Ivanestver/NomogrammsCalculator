#pragma once
#include "nomogramms/DBObject.h"
#include "BypassRule.h"
#include <vector>
#include "common/macros.h"
#include "Nomogramm.h"

namespace nomogramms
{
	DECL_SHARED(Methodology);

	#define MethodologyClass_ID "A8A4951D-8542-4CFA-B593-ECBA3DE727D1"
	class Methodology : public DBObject
	{
		DECL_DBCLASS(Methodology);
	public:
		Methodology(const QUuid& id);

		bool GetChildren(std::vector<SDBObject>& children) const override;

		bool operator==(const DBObject& other) const override;
		bool operator!=(const DBObject& other) const override;

		const std::vector<SNomogramm>& GetNomogramms() const;

	private:
		// Унаследовано через DBObject
		virtual void initFromDB() override;

	private:
		std::vector<SNomogramm> nomogramms;
	};
}