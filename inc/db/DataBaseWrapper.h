#pragma once
#include <QtSql/QSqlDatabase>
#include "common/macros.h"
#include <QUuid>
#include <QVariant>
#include <vector>
#include <memory>
#include <tuple>
#include "db_state/db_state.h"

namespace db
{
	DECL_UNIQUE(DataBaseWrapper);
	DECL_SHARED(DataBaseWrapper);

	// { net_name, net_file, net_id }
	using NNModelInfo = std::tuple<QString, QString, QUuid>;

	class DataBaseWrapper
	{
	public:
		static SDataBaseWrapper GetDatabase();
		~DataBaseWrapper() = default;

		_NODISCARD std::vector<QUuid> GetTemplateIDsByClassID(const QUuid& classID) const;
		_NODISCARD QString GetPropertyValueByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID, QString& error);
		_NODISCARD std::vector<QString> GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& templateID);

		bool AddNN(const QString& nnName, const QString& nnFileName, QString& error);
		_NODISCARD std::vector<NNModelInfo> GetNNModels();
		_NODISCARD NNModelInfo GetNNModelInfo(const QUuid& ModelID);

		bool ExecuteUpdate(const QString& query, const std::vector<QVariant>& params, QString& error);
		_NODISCARD 	std::vector<std::vector<QVariant>> ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error);

	private: 
		QString turnIDToStr(const QUuid& id) const;

		bool openConnection() const;
		void closeConnection() const;

	private:
		mutable QSqlDatabase db;
		db_state::SDBState dbState{ nullptr };
		DataBaseWrapper(const db_state::SDBState& state);
	};

}