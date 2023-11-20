#include "mysql_db_state.h"

namespace db_state
{
    const QString MySQLDBState::GetDBName() const
    {
        return QString("QMYSQL");
    }

    const QString MySQLDBState::GetConnectionString() const
    {
        return QString("nomogramms_db");
    }

    void MySQLDBState::SetupDatabase(QSqlDatabase& db) const
    {
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("6901");
    }
}