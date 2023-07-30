#pragma once
#include <memory>
#include "db/db_state.h"
#include "xml/xml.h"
#include "ms_access_db_state.h"

namespace db
{
	namespace db_state
	{
		class DBStateFactory
		{
		public:
			static std::shared_ptr<DBState> GetState()
			{
				const auto* xmlConfiguration = xml::XmlConfiguration::GetInstance();
				StateType stateType = (StateType)xmlConfiguration->GetValueByTag("db_state_type").toInt();
				switch (stateType)
				{
				case db::db_state::MSAccess:
				{
					QString pathTofile = xmlConfiguration->GetValueByTag("ms_access_path");
					return std::make_shared<MSAccessDBState>(pathTofile);
				}
				case db::db_state::MySQL:
					return SDBState();
				case db::db_state::MSSQLServer:
					return SDBState();
				default:
					return SDBState();
				}
			}
		};
	}
}