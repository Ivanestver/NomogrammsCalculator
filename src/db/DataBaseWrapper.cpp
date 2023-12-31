#include "db/DataBaseWrapper.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringBuilder>
#include "xml/xml.h"
#include "db_state/db_state_factory.h"
#include <exceptions/db_exception.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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

	std::tuple<int, std::vector<int>, int> DataBaseWrapper::readNNStruct(const QString& netStruct) const
	{
		auto doc = QJsonDocument::fromJson(netStruct.toUtf8());
		if (doc.isNull() || !doc.isObject())
			return {};

		auto obj = doc.object();
		int inputFeatures = obj["input"].toInt();
		int outputFeatures = obj["output"].toInt();

		auto arr = obj["hidden"].toArray();
		std::vector<int> hidden;
		for (const auto& value : arr)
			hidden.push_back(value.toInt());

		return { inputFeatures, hidden, outputFeatures };
	}

	bool DataBaseWrapper::openConnection() const
	{
		db = QSqlDatabase::addDatabase(dbState->GetDBName());
		db.setDatabaseName(dbState->GetConnectionString());
		dbState->SetupDatabase(db);
		return db.open();
	}

	void DataBaseWrapper::closeConnection() const
	{
		if (db.isOpen())
			db.close();
	}

	SDataBaseWrapper DataBaseWrapper::GetDatabase()
	{
		auto state = db_state::DBStateFactory::GetState();

		return SDataBaseWrapper(new DataBaseWrapper(state));
	}

	std::vector<QUuid> DataBaseWrapper::GetTemplateIDsByClassID(const QUuid& classID) const
	{
		if (!openConnection())
			throw std::exception("�� ������� ������� �����������");

		QString sID = classID.toString();
		sID = sID.left(sID.length() - 1);
		sID = sID.right(sID.length() - 1).toUpper();

		QSqlQuery query(db);
		query.prepare("select template_id from template where class_id in (?)");
		query.addBindValue(sID);
		if (!query.exec())
			throw std::exception(query.lastError().text().toLocal8Bit());

		std::vector<QUuid> ids;
		while (query.next())
		{
			auto var = query.value(0);
			if (!var.isValid())
				continue;
			ids.push_back(QUuid(var.toString()));
		}

		closeConnection();

		return ids;
	}

	QString DataBaseWrapper::GetPropertyValueByIdAndTemplateID(const QUuid& propertyID, const QUuid& templateID, QString& error)
	{
		if (!openConnection())
			throw std::exception("�� ������� ������� �����������");

		QSqlQuery query(db);
		query.prepare("select property_value from template_property where property_id in (?) and template_id in (?)");
		query.addBindValue(turnIDToStr(propertyID));
		query.addBindValue(turnIDToStr(templateID));
		if (!query.exec())
			throw std::exception(query.lastError().text().toLocal8Bit());

		if (!query.next())
			return "";

		auto var = query.value(0);
		if (!var.isValid())
			throw std::exception("�������� ���������");

		closeConnection();

		return var.toString();
	}

	std::vector<QString> DataBaseWrapper::GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& objID)
	{
		if (!openConnection())
			throw std::exception("�� ������� ������� �����������");

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
			throw std::exception(query.lastError().text().toLocal8Bit());

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
			throw std::exception("�� ������� ������� �����������");

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
			throw std::exception("�� ������� ������� �����������");

		std::vector<NNModelInfo> models;
		QString queryStr("select net_name, net_file, net_id, net_struct from nets");
		QString error;
		auto response = ExecuteQuery(queryStr, {}, error);
		for (const auto& row : response)
		{
			if (row.size() != 4ull)
				continue;

			auto [input, hidden, output] = readNNStruct(row[3].toString());

			models.emplace_back(row[0].toString(), row[1].toString(), row[2].toUuid(), input, output, hidden);
		}

		closeConnection();

		return models;
	}

	NNModelInfo DataBaseWrapper::GetNNModelInfo(const QUuid& ModelID)
	{
		if (!openConnection())
			throw std::exception("�� ������� ������� �����������");
		
		QString error;

		auto response = ExecuteQuery("select sub_id from template_template where master_id = ?", { ModelID }, error);
		QUuid modelId;
		if (response.empty() || response.front().empty())
		{
			modelId = ModelID;
		}
		else
		{
			modelId = response.front().front().toUuid();
		}

		response = ExecuteQuery("select * from nets where net_id = ?", { modelId }, error);

		if (response.empty())
			throw std::exception(QString::fromLocal8Bit("�� ������� ����� ��������� ���� � ����� Id: %1").arg(modelId.toString()).toLocal8Bit());
		
		auto& row = response.front();
		if (row.empty())
			throw std::exception(QString::fromLocal8Bit("�� ������� ����� ��������� ���� � ����� Id: %1").arg(modelId.toString()).toLocal8Bit());

		auto [inputFeatures, hidden, outputFeatures] = readNNStruct(row[3].toString());
		return { row[1].toString(), row[2].toString(), row[0].toUuid(), inputFeatures, outputFeatures, hidden };
	}

	bool DataBaseWrapper::ExecuteUpdate(const QString& query, const std::vector<QVariant>& params, QString& error)
	{
		if (!openConnection())
		{
			error = QString::fromLocal8Bit("�� �������� ������� �����������");
			return false;
		}

		QSqlQuery q(db);
		q.prepare(query);
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
			return false;
		}

		closeConnection();

		return true;
	}

	std::vector<std::vector<QVariant>> DataBaseWrapper::ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error) const
	{
		if (!openConnection())
			throw std::exception("�� ������� ������� �����������");

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
			throw std::exception(q.lastError().text().toLocal8Bit());

		int columnsCount = 0;
		{
			auto record = q.record();
			if (record.isEmpty())
			{
				throw std::exception("No result has been obtained");
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

	QImage DataBaseWrapper::LoadNomogrammPicture(const QUuid& NomogrammId, QString& error) const
	{
		std::vector<std::vector<QVariant>> response = ExecuteQuery("select picture, picture_format from nomogramms_images where template_id = ?", { NomogrammId }, error);

		if (response.empty() || response[0].empty())
		{
			error = QString::fromLocal8Bit("�� ������� �������� ������");
			return {};
		}

		auto pictureAsVariant = response.front()[0];
		if (pictureAsVariant.isNull())
			return {};

		auto pictureFormat = response.front()[1].toString();

		return QImage::fromData(QByteArray::fromBase64(pictureAsVariant.toByteArray()), pictureFormat.toLocal8Bit().constData());
	}
}