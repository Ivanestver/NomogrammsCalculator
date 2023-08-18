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

QString DBExecutor::ExecSELECT(const QString& queryStr, const std::vector<QVariant>& params, std::vector<std::vector<QVariant>>& results) const
{
	results.clear();

	QSqlQuery query(db);
	query.prepare(queryStr);
	for (const auto& param : params)
		query.addBindValue(param);

	if (!query.exec())
		return query.lastError().text();

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

	return "";
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

	query.exec();
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
