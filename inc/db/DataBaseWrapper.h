#pragma once
#include <QtSql/QSqlDatabase>
#include "common/macros.h"
#include "common/DBObject.h"
#include <QUuid>
#include <vector>
#include <memory>
#include "db_state.h"

namespace db
{
	DECL_UNIQUE(DataBaseWrapper);

	class DataBaseWrapper
	{
	public:
		static std::unique_ptr<DataBaseWrapper> Create(const db_state::IDBState* state);
		~DataBaseWrapper();

		QString GetTemplateIDsByClassID(const QUuid& classID, std::vector<QUuid>& ids) const;
		QString GetAttributeByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID) const;
		QString GetPropertiesByIDsAndObjID(std::vector<QString>& values, const std::vector<QUuid>& attributes, const QUuid& templateID) const;

		QString ExecuteUpdate(const QString& query, const std::vector<QVariant>& params) const;
		std::vector<std::vector<QVariant> > ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error) const;

	private:
		QString turnIDToStr(const QUuid& id) const;

	private:
		QSqlDatabase db;
		DataBaseWrapper(const db_state::IDBState* state);
	};

}