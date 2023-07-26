#pragma once
#include "common/DBObject.h"
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
		Methodology(const Methodology& other);
		~Methodology() override;
		// Унаследовано через DBObject
		virtual void initFromDB() override;

		bool operator==(const DBObject& other) override;
		bool operator!=(const DBObject& other) override;
		Methodology& operator=(const DBObject& other) override;

	private:
		std::vector<SNomogramm> nomogramms;
	};
}