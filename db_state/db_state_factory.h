#pragma once
#include <memory>
#include "db_state.h"
#include "xml/xml.h"
#include "ms_access_db_state.h"
#include "mysql_db_state.h"

namespace db_state
{
	class DBStateFactory
	{
	public:
		static std::shared_ptr<DBState> GetState()
		{
			const auto* xmlConfiguration = xml::XmlConfiguration::GetInstance();
			auto value = xmlConfiguration->GetValueByTag("db_state_type");
			if (value.isEmpty())
				return SDBState();

			StateType stateType = (StateType)value.toInt();
			switch (stateType)
			{
			case db_state::MSAccess:
			{
				QString pathTofile = xmlConfiguration->GetValueByTag("db_path");
				return std::make_shared<MSAccessDBState>(pathTofile);
			}
			case db_state::MySQL:
				return std::make_shared<MySQLDBState>();
			case db_state::MSSQLServer:
				return SDBState();
			default:
				return SDBState();
			}
		}
	};
}