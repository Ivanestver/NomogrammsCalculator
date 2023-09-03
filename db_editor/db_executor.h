#pragma once
#include <QSqlDatabase>
#include <db_state.h>
#include <memory>
#include <map>
#include <tuple>
#include <vector>
#include <set>
#include <QString>
#include <QVariant>
#include <QUuid>

class DBExecutor
{
public:
	using Record = std::vector<QVariant>;
	using Response = std::vector<Record>;
	using PropertyInfo = std::tuple<QUuid, QString, QVariant>;

	struct DBExecutorUtils
	{
		static QString TurnUuidToStr(const QUuid& id);
	};

public:
	static std::shared_ptr<DBExecutor> GetInstance();
	~DBExecutor();

	bool ExecSELECT(const QString& queryStr, const std::vector<QVariant>& params, Response& results, QString& error) const;
	int ExecChange(const QString& queryStr, const std::vector<QVariant>& params, QString& error) const;
	bool RemoveTemplate(const QUuid& templateId, QString& error) const;

	bool InsertNewTemplate(const QUuid& templateId, const QUuid& classId, QString& error) const;

	bool InsertProperty(const QUuid& templateId, const QUuid& propertyId, const QVariant& value, QString& error) const;
	bool UpdateProperty(const QUuid& templateId, const QUuid& propertyId, const QVariant& value, QString& error) const;

	bool LinkTemplates(const QUuid& masterObjId, const QUuid& subObjId, QString& error) const;

	bool ReceivePropertiesOfObj(const QUuid& objId, std::set<PropertyInfo>& properties, QString& error) const;
	bool ReceiveProperties(const std::vector<QUuid>& propertiesId, std::set<PropertyInfo>& properoties, QString& error) const;

private:
	int removeTemplateFromTable(const QUuid& templateId, const QString& table, const QString& fieldOfTemplate, QString& error) const;

private:
	QSqlDatabase db;
	DBExecutor(const db_state::SDBState& state);
};