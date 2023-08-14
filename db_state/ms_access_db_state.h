#pragma once

#include <QString>
#include <db_state/db_state.h>

namespace db_state
{
	class MSAccessDBState : public DBState
	{
	public:
		MSAccessDBState(const QString& pathToFile);
		~MSAccessDBState() override = default;
		// Унаследовано через IDBState
		virtual const QString GetDBName() const override;
		virtual const QString GetConnectionString() const override;

	private:
		const QString pathToMSAccessFile;
		MSAccessDBState(const MSAccessDBState&) = delete;
		MSAccessDBState& operator=(const MSAccessDBState&) = delete;
	};
}