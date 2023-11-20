#pragma once

#include "db_state/db_state.h"
#include <QString>

namespace db_state
{
	class MySQLDBState : public DBState
	{
	public:
		~MySQLDBState() override = default;

		// Унаследовано через DBState
		const QString GetDBName() const override;
		const QString GetConnectionString() const override;
		void SetupDatabase(QSqlDatabase& db) const override;
	};
}