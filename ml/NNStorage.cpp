#include "NNStorage.h"
#include "db/DataBaseWrapper.h"
#include "xml/xml.h"
#include <QDebug>

namespace ml
{
	NNStorage& NNStorage::GetInstance()
	{
		static NNStorage instance;
		return instance;
	}

	SFullyConnectedNN NNStorage::GetNet(const QUuid& NetID) const
	{
		const auto it = nets.find(NetID);
		if (it == nets.end())
			return loadNN(NetID);

		return it->second;
	}

	bool NNStorage::RemoveNet(const QUuid& NetID)
	{
		const auto it = nets.find(NetID);
		if (it == nets.end())
			return false;

		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return false;

		auto nnInfo = db->GetNNModelInfo(NetID);
		try
		{
			const auto* xmlConfig = xml::XmlConfiguration::GetInstance();
			QString path = xmlConfig->GetValueByTag("models_path");
			path += std::get<1>(nnInfo);
			QFile fileToRemove(path);
			if (!fileToRemove.exists())
				return false;

			return fileToRemove.remove();
		}
		catch (std::exception& e)
		{
			qDebug() << e.what();
			return false;
		}
	}

	SFullyConnectedNN NNStorage::loadNN(const QUuid& NetID) const
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return {};

		auto nnInfo = db->GetNNModelInfo(NetID);

		SFullyConnectedNN nn = std::make_shared<FullyConnectedNN>(1, std::vector<int>(), 1, std::get<0>(nnInfo));
		torch::serialize::InputArchive inputArchive;

		try
		{
			const auto* xmlConfig = xml::XmlConfiguration::GetInstance();
			QString path = xmlConfig->GetValueByTag("models_path");
			path += std::get<1>(nnInfo);
			inputArchive.load_from(path.toStdString());
			nn->load(inputArchive);
			nets.insert({ NetID, nn });
		}
		catch (std::exception& e)
		{
			qDebug() << e.what();
			return {};
		}

		return nn;
	}
}