#include "ms_access_db_state.h"

namespace db_state
{
	MSAccessDBState::MSAccessDBState(const QString& pathToFile)
		: pathToMSAccessFile(pathToFile)
	{}

	const QString MSAccessDBState::GetDBName() const
	{
		return QString("QODBC");
	}

	const QString MSAccessDBState::GetConnectionString() const
	{
		return QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=%1")
			.arg(pathToMSAccessFile);
	}

	void MSAccessDBState::SetupDatabase(QSqlDatabase& db) const
	{
	}
}