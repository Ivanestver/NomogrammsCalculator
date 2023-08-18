#pragma once
#include <QSqlDatabase>
#include <db_state.h>
#include <memory>
#include <vector>
#include <QString>
#include <QVariant>
#include <QUuid>

class DBExecutor
{
public:
	struct DBExecutorUtils
	{
		static QString TurnUuidToStr(const QUuid& id);
	};

public:
	static std::shared_ptr<DBExecutor> GetInstance();
	~DBExecutor();

	QString ExecSELECT(const QString& queryStr, const std::vector<QVariant>& params, std::vector<std::vector<QVariant>>& results) const;
	int ExecChange(const QString& queryStr, const std::vector<QVariant>& params, QString& error) const;
	bool RemoveTemplate(const QUuid& templateId, QString& error) const;

private:
	int removeTemplateFromTable(const QUuid& templateId, const QString& table, const QString& fieldOfTemplate, QString& error) const;

private:
	QSqlDatabase db;
	DBExecutor(const db_state::SDBState& state);
};