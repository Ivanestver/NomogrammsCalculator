#include "db_executor.h"
#include <db_state_factory.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

std::shared_ptr<DBExecutor> DBExecutor::GetInstance()
{
	auto state = db_state::DBStateFactory::GetState();
	return std::shared_ptr<DBExecutor>(new DBExecutor(state));
}

DBExecutor::~DBExecutor()
{
	if (db.isOpen())
		db.close();
}

bool DBExecutor::ExecSELECT(const QString& queryStr, const std::vector<QVariant>& params, Response& results, QString& error) const
{
	results.clear();

	QSqlQuery query(db);
	query.prepare(queryStr);
	for (const auto& param : params)
	{
		if (param.type() == QVariant::Type::Uuid)
			query.addBindValue(DBExecutorUtils::TurnUuidToStr(param.toUuid()));
		else
			query.addBindValue(param);
	}

	if (!query.exec())
	{
		error = query.lastError().text();
		return false;
	}

	while (query.next())
	{
		auto result = query.record();
		std::vector<QVariant> ress;
		for (int i = 0; i < result.count(); ++i)
		{
			auto res = result.value(i);
			if (res.isValid())
				ress.push_back(std::move(res));
		}

		results.push_back(std::move(ress));
	}

	return true;
}

int DBExecutor::ExecChange(const QString& queryStr, const std::vector<QVariant>& params, QString& error) const
{
	QSqlQuery query(db);
	query.prepare(queryStr);
	for (const auto& param : params)
	{
		if (param.type() == QVariant::Type::Uuid)
			query.addBindValue(DBExecutorUtils::TurnUuidToStr(param.toUuid()));
		else
			query.addBindValue(param);
	}

	if (!query.exec())
		error = query.lastError().text();

	return query.numRowsAffected();
}

bool DBExecutor::RemoveTemplate(const QUuid& templateId, QString& error) const
{
	int numRowsAffected = removeTemplateFromTable(templateId, QString("template_property"), QString("template_id"), error);
	numRowsAffected += removeTemplateFromTable(templateId, QString("template"), QString("template_id"), error);
	numRowsAffected += removeTemplateFromTable(templateId, QString("template_template"), QString("sub_id"), error);
	numRowsAffected += removeTemplateFromTable(templateId, QString("template_template"), QString("master_id"), error);

	return numRowsAffected > 0;
}

bool DBExecutor::InsertNewTemplate(const QUuid& templateId, const QUuid& classId, QString& error) const
{
	int rowsInserted = ExecChange("insert into [template](template_id, class_id) values (?, ?)", { templateId, classId }, error);
	return rowsInserted > 0;
}

bool DBExecutor::InsertProperty(const QUuid& templateId, const QUuid& propertyId, const QVariant& value, QString& error) const
{
	int rowsInserted = ExecChange("insert into [template_property](template_id, property_id, property_value) values (?, ?, ?)", { templateId, propertyId, value }, error);
	return rowsInserted > 0;
}

bool DBExecutor::UpdateProperty(const QUuid& templateId, const QUuid& propertyId, const QVariant& value, QString& error) const
{
	int rowsInserted = ExecChange("update [template_property] set [property_value] = ? where [template_id] = ? and [property_id] = ?", { value, templateId, propertyId }, error);
	return rowsInserted > 0;
}

bool DBExecutor::LinkTemplates(const QUuid& masterObjId, const QUuid& subObjId, QString& error) const
{
	int rowsInserted = ExecChange("insert into [template_template](master_id, sub_id, extra) values (?, ?, ?)", { masterObjId, subObjId, QString("") }, error);
	return rowsInserted > 0;
}

bool DBExecutor::ReceivePropertiesOfObj(const QUuid& objId, std::set<PropertyInfo>& properties, QString& error) const
{
	Response results;
	if (!ExecSELECT("select t1.property_id, t1.property_name, t2.property_value from [property] as t1 inner join [template_property] as t2 on t1.property_id=t2.property_id where [template_id] = ?", { objId }, results, error))
		return false;

	for (const auto& record : results)
		properties.insert({record[0].toUuid(), record[1].toString(), record[2]});

	return true;
}

bool DBExecutor::ReceiveProperties(const std::vector<QUuid>& propertiesId, std::set<PropertyInfo>& properoties, QString& error) const
{
	std::vector<QVariant> params;
	for (const auto& propertyId : propertiesId)
		params.push_back(propertyId);

	Response results;
	if (!ExecSELECT("select", params, results, error))
		return false;

	return true;
}

int DBExecutor::removeTemplateFromTable(const QUuid& templateId, const QString& table, const QString& fieldOfTemplate, QString& error) const
{
	QString queryStr = QString("delete from [%1] where [%2] = ?").arg(table).arg(fieldOfTemplate);
	QSqlQuery query(db);
	query.prepare(queryStr);
	query.addBindValue(DBExecutorUtils::TurnUuidToStr(templateId));

	if (!query.exec())
	{
		error = query.lastError().text();
		return 0;
	}

	return query.numRowsAffected();
}

DBExecutor::DBExecutor(const db_state::SDBState& state)
{
	db = QSqlDatabase::addDatabase(state->GetDBName());
	db.setDatabaseName(state->GetConnectionString());
	qDebug() << db.open();
}

QString DBExecutor::DBExecutorUtils::TurnUuidToStr(const QUuid& id)
{
	QString idStr = id.toString();
	idStr = idStr.left(idStr.length() - 1);
	idStr = idStr.right(idStr.length() - 1);
	return idStr;
}
