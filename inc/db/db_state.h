#pragma once
#include <QString>
#include <memory>
#include "common/macros.h"

namespace db
{
	namespace db_state
	{
		enum StateType
		{
			MSAccess,
			MySQL,
			MSSQLServer
		};

		DECL_SHARED(DBState);

		class DBState
		{
		public:
			virtual ~DBState() = 0 {};

			virtual const QString GetDBName() const = 0;
			virtual const QString GetConnectionString() const = 0;

		protected:
			DBState() = default;

		private:
			DBState(const DBState& other) = default;
			DBState& operator=(const DBState& other) = default;
			DBState(DBState&& other) = default;
			DBState& operator=(DBState&& other) = default;
		};
	}
}