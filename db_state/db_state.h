#pragma once
#include <QString>
#include <memory>
#include <QtSql/QSqlDatabase>

namespace db_state
{
	enum StateType
	{
		MSAccess,
		MySQL,
		MSSQLServer
	};

	class DBState; using SDBState = std::shared_ptr<DBState>;

	class DBState
	{
	public:
		virtual ~DBState() = 0 {};

		virtual const QString GetDBName() const = 0;
		virtual const QString GetConnectionString() const = 0;
		virtual void SetupDatabase(QSqlDatabase& db) const = 0;

	protected:
		DBState() = default;

	private:
		DBState(const DBState& other) = default;
		DBState& operator=(const DBState& other) = default;
		DBState(DBState&& other) = default;
		DBState& operator=(DBState&& other) = default;
	};
}