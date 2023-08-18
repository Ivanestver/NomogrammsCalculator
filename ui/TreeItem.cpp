#include "TreeItem.h"

namespace ui
{
	TreeItem::TreeItem(QObject* parent)
		: QObject(parent)
	{
	}

	TreeItem::TreeItem(const TreeItem& Other)
	{
		*this = Other;
	}

	TreeItem::TreeItem(const SDBObject& item_, const TreeItem* parent_)
		: item(item_)
		, parent(parent_)
	{}

	TreeItem& TreeItem::operator=(const TreeItem& Other)
	{
		this->item = Other.item;
		this->parent = Other.parent;

		this->children.clear();
		this->children.insert(this->children.begin(), Other.children.begin(), Other.children.end());

		return *this;
	}

	bool operator==(const TreeItem& left, const TreeItem& right)
	{
		return *left.item == *right.item
			&& left.parent == right.parent;
	}
}