#include "db/DataBaseWrapper.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringBuilder>
#include "xml/xml.h"

namespace db
{
	DataBaseWrapper::DataBaseWrapper(const db_state::IDBState* state)
	{
		db = QSqlDatabase::addDatabase(state->GetDBName());
		db.setDatabaseName(state->GetConnectionString());
		qDebug() << db.open();
	}

	QString DataBaseWrapper::turnIDToStr(const QUuid& id) const
	{
		QString s = id.toString();
		s = s.left(s.length() - 1);
		s = s.right(s.length() - 1);
		return s;
	}

	std::unique_ptr<DataBaseWrapper> DataBaseWrapper::Create(const db_state::IDBState* state)
	{
		if (!state)
			return std::unique_ptr<DataBaseWrapper>();

		return std::make_unique<DataBaseWrapper>(state);
	}

	DataBaseWrapper::~DataBaseWrapper()
	{
		if (db.isOpen())
			db.close();
	}

	QString DataBaseWrapper::GetTemplateIDsByClassID(const QUuid& classID, std::vector<QUuid>& ids) const
	{
		QString sID = classID.toString();
		sID = sID.left(sID.length() - 1);
		sID = sID.right(sID.length() - 1).toUpper();
		QSqlQuery query;
		query.prepare("select template_id from template where class_id = :classID");
		query.bindValue(":classID", sID);
		if (!query.exec())
			return query.lastError().text();

		while (query.next())
		{
			auto var = query.value(0);
			if (!var.isValid())
				continue;
			ids.push_back(QUuid(var.toString()));
		}

		return "";
	}

	QString DataBaseWrapper::GetAttributeByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID) const
	{
		return QString();
	}

	QString DataBaseWrapper::GetPropertiesByIDsAndObjID(std::vector<QString>& values, const std::vector<QUuid>& attributes, const QUuid& objID) const
	{
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
			return query.lastError().text();

		while (query.next())
		{
			auto var = query.value(0);
			if (!var.isValid())
				continue;
			values.push_back(var.value<QString>());
		}

		return "";
	}

	QString DataBaseWrapper::ExecuteUpdate(const QString& query, const std::vector<QVariant>& params) const
	{
		QSqlQuery q(db);
		q.prepare(query);
		for (size_t i = 0; i < params.size(); i++)
			q.bindValue(i, params[i]);

		if (!q.exec())
			return q.lastError().text();

		return QString();
	}

	std::vector<std::vector<QVariant> > DataBaseWrapper::ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error) const
	{
		error = "";
		std::vector<std::vector<QVariant> > result;
		QSqlQuery q(db);
		if (!q.prepare(query))
		{
			error = q.lastError().text();
			return result;
		}

		for (size_t i = 0; i < params.size(); i++)
			q.bindValue(i, params[i]);

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
				auto var = q.value(0);
				row.emplace_back(var);
			}
			result.emplace_back(row);
		}

		return result;
	}
}