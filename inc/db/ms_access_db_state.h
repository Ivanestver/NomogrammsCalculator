#pragma once

#include <QString>
#include <db/db_state.h>

namespace db
{
	namespace db_state
	{
		class MSAccessDBState : public IDBState
		{
		public:
			MSAccessDBState(const QString& pathToFile);
			~MSAccessDBState() override = default;
			// Унаследовано через IDBState
			virtual const QString GetDBName() const override;
			virtual const QString GetConnectionString() const override;

		private:
			const QString pathToMSAccessFile;
		};
	}
}