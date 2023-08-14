#include "TreeViewModel.h"
#include "db_executor.h"
#include <QDebug>
#include "db_state/properties.h"

TreeViewModel::TreeViewModel(const std::vector<TreeItem>& items_, QObject* parent/* = nullptr*/)
    : QAbstractItemModel(parent)
    , items(items_)
{}

Q_INVOKABLE QModelIndex TreeViewModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        if (items.empty())
            return Q_INVOKABLE QModelIndex();

        return Q_INVOKABLE createIndex(row, column, (void*)&items[row]);
    }

    auto* parentPtr = reinterpret_cast<TreeItem*>(parent.internalPointer());
    if (parentPtr->children.empty())
    {
        auto executor = DBExecutor::GetInstance();
        std::vector<std::vector<QVariant>> results;
        QString queryStr = "select t1.sub_id, master_id, property_value\
 from(template_template as t1\
 inner join template_property as t2\
 on t1.sub_id = t2.template_id)\
 inner join template as t3\
 on t1.sub_id = t3.template_id\
 where master_id = ?\
 and property_id = ?";
        QString error = executor->ExecSELECT(queryStr, { DBExecutor::DBExecutorUtils::TurnUuidToStr(parentPtr->id), DBExecutor::DBExecutorUtils::TurnUuidToStr(db_state::properties::dbobject_name)}, results);
        if (!error.isEmpty())
        {
            qDebug() << error;
            return Q_INVOKABLE QModelIndex();
        }

        for (const auto& result : results)
        {
            TreeItem newTreeItem;
            newTreeItem.id = result[0].toUuid();
            newTreeItem.classId = result[1].toUuid();
            newTreeItem.name = result[2].toString();
            newTreeItem.parent = parentPtr;
            parentPtr->children.push_back(newTreeItem);
        }
    }

    return Q_INVOKABLE createIndex(row, column, (void*)&parentPtr->children[row]);
}

Q_INVOKABLE QModelIndex TreeViewModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return Q_INVOKABLE QModelIndex();

    const auto* childPtr = reinterpret_cast<const TreeItem*>(child.internalPointer());
    if (!childPtr)
        return Q_INVOKABLE QModelIndex();

    const auto* parentPtr = childPtr->parent;
    if (!parentPtr)
        return Q_INVOKABLE QModelIndex();

    const auto it = std::find_if(parentPtr->children.begin(), parentPtr->children.end(), [childPtr](const TreeItem& item)
        {
            return *childPtr == item;
        });

    if (it == parentPtr->children.end())
        return Q_INVOKABLE QModelIndex();

    return Q_INVOKABLE createIndex(it - parentPtr->children.begin(), 0, (void*)childPtr);
}

Q_INVOKABLE int TreeViewModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return Q_INVOKABLE (int)items.size();

    const auto* item = reinterpret_cast<const TreeItem*>(parent.internalPointer());

    return Q_INVOKABLE (int)item->children.size();
}

Q_INVOKABLE int TreeViewModel::columnCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE 1;
}

Q_INVOKABLE QVariant TreeViewModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return Q_INVOKABLE QVariant();

    const auto* item = reinterpret_cast<const TreeItem*>(index.internalPointer());
    return Q_INVOKABLE item->name;
}