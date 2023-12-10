#include "db/DataBaseWrapper.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringBuilder>
#include "xml/xml.h"
#include "db_state/db_state_factory.h"
#include <exceptions/db_exception.h>

namespace db
{
	DataBaseWrapper::DataBaseWrapper(const db_state::SDBState& state)
		: dbState(state)
	{}

	QString DataBaseWrapper::turnIDToStr(const QUuid& id) const
	{
		QString s = id.toString();
		s = s.left(s.length() - 1);
		s = s.right(s.length() - 1);
		return s;
	}

	bool DataBaseWrapper::openConnection()
	{
		db = QSqlDatabase::addDatabase(dbState->GetDBName());
		db.setDatabaseName(dbState->GetConnectionString());
		dbState->SetupDatabase(db);
		return db.open();
	}

	void DataBaseWrapper::closeConnection()
	{
		if (db.isOpen())
			db.close();
	}

	SDataBaseWrapper DataBaseWrapper::GetDatabase()
	{
		auto state = db_state::DBStateFactory::GetState();

		return SDataBaseWrapper(new DataBaseWrapper(state));
	}

	QString DataBaseWrapper::GetTemplateIDsByClassID(const QUuid& classID, std::vector<QUuid>& ids)
	{
		if (!openConnection())
			return QString::fromLocal8Bit("Не удалость открыть подключение");

		QString sID = classID.toString();
		sID = sID.left(sID.length() - 1);
		sID = sID.right(sID.length() - 1).toUpper();

		QSqlQuery query(db);
		query.prepare("select template_id from template where class_id in (?)");
		query.addBindValue(sID);
		if (!query.exec())
			return query.lastError().text();

		while (query.next())
		{
			auto var = query.value(0);
			if (!var.isValid())
				continue;
			ids.push_back(QUuid(var.toString()));
		}

		closeConnection();

		return "";
	}

	QString DataBaseWrapper::GetPropertyValueByIdAndTemplateID(const QUuid& propertyID, const QUuid& templateID, QString& error)
	{
		if (!openConnection())
		{
			error = QString::fromLocal8Bit("Не удалость открыть подключение");
			return QString();
		}

		QSqlQuery query(db);
		query.prepare("select property_value from template_property where property_id in (?) and template_id in (?)");
		query.addBindValue(turnIDToStr(propertyID));
		query.addBindValue(turnIDToStr(templateID));
		if (!query.exec())
			throw exceptions::BadRequestException(query.lastError().text());

		if (!query.next())
			return "";

		auto var = query.value(0);
		if (!var.isValid())
		{
			throw exceptions::BadRequestException("Значение невалидно");
		}

		closeConnection();

		return var.toString();
	}

	std::vector<QString> DataBaseWrapper::GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& objID)
	{
		if (!openConnection())
			return {};

		std::vector<QString> values;
		QString queryStr = QString("select property_value from template_property where template_id = :tID and property_id in (");
		for (size_t i = 0; i < attributes.size(); i++)
		{
			queryStr += QString(":%1").arg(i);
		}

		queryStr = queryStr.left(queryStr.length());
		queryStr += ")";

		QSqlQuery query(db);
		query.prepare(queryStr);
		query.bindValue(":tID", objID.toString());
		for (size_t i = 0; i < attributes.size(); i++)
			query.bindValue(QString(":%1").arg(i), turnIDToStr(attributes[i].toString()));

		if (!query.exec())
			throw exceptions::BadRequestException(query.lastError().text());

		while (query.next())
		{
			auto var = query.value(0);
			if (!var.isValid())
				continue;
			values.push_back(var.value<QString>());
		}

		closeConnection();

		return values;
	}

	bool DataBaseWrapper::AddNN(const QString& nnName, const QString& nnFileName, QString& error)
	{
		if (!openConnection())
			return false;

		QString queryStr = QString("insert into nets values(?, ?, ?)");
		QSqlQuery query(db);
		query.prepare(queryStr);
		query.addBindValue(turnIDToStr(QUuid::createUuid()));
		query.addBindValue(nnName);
		query.addBindValue(nnFileName);

		if (!query.exec())
		{
			error = query.lastError().text();
			return false;
		}

		closeConnection();

		return true;
	}

	std::vector<NNModelInfo> DataBaseWrapper::GetNNModels()
	{
		if (!openConnection())
			return {};

		std::vector<NNModelInfo> models;
		QString queryStr("select net_name, net_file, net_id from nets");
		QString error;
		auto response = ExecuteQuery(queryStr, {}, error);
		for (const auto& row : response)
		{
			if (row.size() != 3ull)
				continue;

			models.emplace_back(row[0].toString(), row[1].toString(), row[2].toUuid());
		}

		closeConnection();

		return models;
	}

	NNModelInfo DataBaseWrapper::GetNNModelInfo(const QUuid& ModelID)
	{
		if (!openConnection())
			return {};
		
		QString error;
		auto response = ExecuteQuery("select * from nets where net_id = ?", { ModelID }, error);

		if (response.empty())
			return {};
		
		auto& row = response.front();
		if (row.empty())
			return {};

		return { row[1].toString(), row[2].toString(), row[0].toUuid() };
	}

	bool DataBaseWrapper::ExecuteUpdate(const QString& query, const std::vector<QVariant>& params, QString& error)
	{
		if (!openConnection())
		{
			error = QString::fromLocal8Bit("Не удалость открыть подключение");
			return false;
		}

		QSqlQuery q(db);
		q.prepare(query);
		for (size_t i = 0; i < params.size(); i++)
			q.bindValue((int)i, params[i]);

		if (!q.exec())
		{
			error = q.lastError().text();
			return false;
		}

		closeConnection();

		return true;
	}

	std::vector<std::vector<QVariant>> DataBaseWrapper::ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error)
	{
		if (!openConnection())
		{
			error = QString::fromLocal8Bit("Не удалость открыть подключение");
			return {};
		}
		std::vector<std::vector<QVariant> > result;
		QSqlQuery q(db);
		if (!q.prepare(query))
		{
			error = q.lastError().text();
			return result;
		}

		for (const auto& param : params)
		{
			if (param.type() == QVariant::Type::Uuid)
				q.addBindValue(turnIDToStr(param.toUuid()));
			else
				q.addBindValue(param);
		}

		if (!q.exec())
		{
			error = q.lastError().text();
			return result;
		}

		int columnsCount = 0;
		{
			auto record = q.record();
			if (record.isEmpty())
			{
				error = "No result has been obtained";
				return result;
			}
			columnsCount = record.count();
		}

		while (q.next())
		{
			std::vector<QVariant> row;
			for (int i = 0; i < columnsCount; i++)
			{
				auto var = q.value(i);
				row.emplace_back(var);
			}
			result.emplace_back(row);
		}

		closeConnection();

		return result;
	}
}