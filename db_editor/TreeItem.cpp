#include "TreeItem.h"

TreeItem::TreeItem(QObject* parent)
	: QObject(parent)
{
}

TreeItem::TreeItem(const TreeItem& Other)
{
	*this = Other;
}

TreeItem& TreeItem::operator=(const TreeItem& Other)
{
	this->id = Other.id;
	this->children = Other.children;

	return *this;
}

TreeItem::operator QVariant() const
{
	QVariant v;
	v.setValue(v);
	v.setValue(children);

	return v;
}

bool operator==(const TreeItem& left, const TreeItem& right)
{
	return left.id == right.id
		&& left.classId == right.classId;
}
