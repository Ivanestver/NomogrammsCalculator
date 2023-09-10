#pragma once

#include <vector>
#include <QUuid>
#include <QVariant>
#include <QString>
#include <QObject>
#include "qmetatype.h"
#include "qmetaobject.h"
#include "nomogramms/DBObject.h"

namespace ui
{
	class TreeItem : public QObject
	{
		Q_OBJECT;

	public:
		TreeItem(QObject* parent = nullptr);
		TreeItem(const TreeItem& Other);
		TreeItem(const SDBObject& item_, const TreeItem* parent_);
		~TreeItem() override = default;

		TreeItem& operator=(const TreeItem& Other);

		SDBObject item = nullptr;
		const TreeItem* parent = nullptr;
		std::vector<TreeItem> children;
	};

	bool operator==(const TreeItem& left, const TreeItem& right);
}

Q_DECLARE_METATYPE(ui::TreeItem)