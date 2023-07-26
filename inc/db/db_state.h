#pragma once
#include <QString>

namespace db
{
	namespace db_state
	{
		class IDBState
		{
		public:
			virtual ~IDBState() = 0 {};

			virtual const QString GetDBName() const = 0;
			virtual const QString GetConnectionString() const = 0;

		protected:
			IDBState() = default;

		private:
			IDBState(const IDBState& other) = default;
			IDBState& operator=(const IDBState& other) = default;
			IDBState(IDBState&& other) = default;
			IDBState& operator=(IDBState&& other) = default;
		};
	}
}