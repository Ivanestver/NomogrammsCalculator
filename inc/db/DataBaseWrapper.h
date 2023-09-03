#pragma once
#include <QtSql/QSqlDatabase>
#include "common/macros.h"
#include <QUuid>
#include <QVariant>
#include <vector>
#include <memory>
#include "db_state/db_state.h"
#include <db_state/db_state.h>

namespace db
{
	DECL_UNIQUE(DataBaseWrapper);
	DECL_SHARED(DataBaseWrapper);

	class DataBaseWrapper
	{
	public:
		static SDataBaseWrapper GetDatabase();
		~DataBaseWrapper() = default;

		QString GetTemplateIDsByClassID(const QUuid& classID, std::vector<QUuid>& ids);
		QString GetPropertyValueByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID);
		std::vector<QString> GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& templateID);

		bool ExecuteUpdate(const QString& query, const std::vector<QVariant>& params, QString& error);
		std::vector<std::vector<QVariant>> ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error);

	private: 
		QString turnIDToStr(const QUuid& id) const;

		bool openConnection();
		void closeConnection();

	private:
		QSqlDatabase db;
		db_state::SDBState dbState = nullptr;
		DataBaseWrapper(const db_state::SDBState& state);
	};

}