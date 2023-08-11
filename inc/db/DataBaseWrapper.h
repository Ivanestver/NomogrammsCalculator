#pragma once
#include <QtSql/QSqlDatabase>
#include "common/macros.h"
#include <QUuid>
#include <QVariant>
#include <vector>
#include <memory>
#include "db_state.h"

namespace db
{
	DECL_UNIQUE(DataBaseWrapper);
	DECL_SHARED(DataBaseWrapper);

	class DataBaseWrapper
	{
	public:
		static SDataBaseWrapper GetDatabase();
		~DataBaseWrapper();

		QString GetTemplateIDsByClassID(const QUuid& classID, std::vector<QUuid>& ids) const;
		QString GetPropertyValueByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID) const;
		std::vector<QString> GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& templateID) const;

		QString ExecuteUpdate(const QString& query, const std::vector<QVariant>& params) const;
		std::vector<std::vector<QVariant> > ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error) const;

	private: 
		QString turnIDToStr(const QUuid& id) const;

	private:
		QSqlDatabase db;
		DataBaseWrapper(const db_state::SDBState& state);
	};

}