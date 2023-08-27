#pragma once

#include <QUuid>
#include <QVariant>
#include <QString>
#include <map>

struct PropertiesTableItem
{
	QUuid id;
	QString name;
	QVariant value;

	PropertiesTableItem() = default;

	PropertiesTableItem(const QUuid& id, const QString& name, const QVariant& value)
		: id(id), name(name), value(value)
	{}
};

struct PropertiesTableItemInfo
{
	QUuid id;
	QUuid classId;
	std::vector<PropertiesTableItem> properties;
};