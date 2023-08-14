#pragma once

#include <vector>
#include <QUuid>
#include <QVariant>
#include <QString>
#include <QObject>
#include "qmetatype.h"
#include "qmetaobject.h"

class TreeItem : public QObject
{
	Q_OBJECT;

public:
	TreeItem(QObject* parent = nullptr);
	TreeItem(const TreeItem& Other);
	~TreeItem() override = default;

	TreeItem& operator=(const TreeItem& Other);

	QUuid id{};
	QUuid classId{};
	QString name{};
	std::vector<TreeItem> children{};
	TreeItem* parent = nullptr;

	operator QVariant() const;
};

bool operator==(const TreeItem& left, const TreeItem& right);

Q_DECLARE_METATYPE(TreeItem)