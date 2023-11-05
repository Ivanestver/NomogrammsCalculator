#pragma once
#include "AbstractNN.h"
#include <memory>
#include <QUuid>
#include <map>

namespace ml
{
	class NNStorage
	{
	public:
		static NNStorage& GetInstance();

		SFullyConnectedNN GetNet(const QUuid& NetID);
		bool RemoveNet(const QUuid& NetID);

	public:
		~NNStorage() = default;

	private:
		NNStorage() = default;

		SFullyConnectedNN loadNN(const QUuid& NetID);

	private:
		std::map<QUuid, SFullyConnectedNN> nets;
	};
}