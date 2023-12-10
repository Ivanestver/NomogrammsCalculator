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

		SFullyConnectedNN GetNet(const QUuid& NetID) const;
		bool RemoveNet(const QUuid& NetID);

	public:
		~NNStorage() = default;

	private:
		NNStorage() = default;

		SFullyConnectedNN loadNN(const QUuid& NetID) const;

	private:
		mutable std::map<QUuid, SFullyConnectedNN> nets;
	};
}