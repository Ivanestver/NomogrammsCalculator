#include "TreeViewModel.h"
#include "db_executor.h"
#include <QDebug>
#include "db_state/properties.h"

TreeItemModel::TreeItemModel(const std::vector<TreeItem>& items_, QObject* parent/* = nullptr*/)
    : QAbstractItemModel(parent)
    , rootItem(new TreeItem())
{
    rootItem->children.insert(rootItem->children.begin(), items_.begin(), items_.end());
    for (auto& item : rootItem->children)
        item.parent = rootItem;
}

Q_INVOKABLE QModelIndex TreeItemModel::index(int row, int column, const QModelIndex& parent) const
{
    auto* parentPtr = getItem(parent);
    /*if (parentPtr->children.empty())
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
    }*/

    if (parentPtr->children.empty())
        return Q_INVOKABLE QModelIndex();

    return Q_INVOKABLE createIndex(row, column, (void*)&parentPtr->children[row]);
}

Q_INVOKABLE QModelIndex TreeItemModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return Q_INVOKABLE QModelIndex();

    const auto* childPtr = getItem(child);
    const auto* parentPtr = childPtr->parent;
    if (!parentPtr || parentPtr == rootItem)
        return Q_INVOKABLE QModelIndex();

    const auto it = std::find_if(parentPtr->children.begin(), parentPtr->children.end(), [childPtr](const TreeItem& item)
        {
            return *childPtr == item;
        });

    if (it == parentPtr->children.end())
        return Q_INVOKABLE QModelIndex();

    return Q_INVOKABLE createIndex(it - parentPtr->children.begin(), 0, (void*)parentPtr);
}

Q_INVOKABLE int TreeItemModel::rowCount(const QModelIndex& parent) const
{
    const auto* item = getItem(parent);
    return Q_INVOKABLE (int)item->children.size();
}

Q_INVOKABLE int TreeItemModel::columnCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE 1;
}

Q_INVOKABLE QVariant TreeItemModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return Q_INVOKABLE QVariant();

    const auto* item = getItem(index);
    return Q_INVOKABLE item->name;
}

Q_INVOKABLE bool TreeItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::DisplayRole
     && role != ModelRole::NameRole
     && role != ModelRole::ClassIDRole)
        return Q_INVOKABLE false;

    switch (role)
    {
    case Qt::DisplayRole:
    case ModelRole::NameRole:
    {
        if (value.type() != QVariant::Type::String)
            return Q_INVOKABLE false;

        auto* item = getItem(index);
        item->name = value.toString();
        break;
    }
    case ModelRole::ClassIDRole:
    {
        if (value.type() != QVariant::Type::Uuid)
            return Q_INVOKABLE false;

        auto* item = getItem(index);
        item->classId = value.toUuid();
        break;
    }
    default:
        return Q_INVOKABLE false;
    }

    return Q_INVOKABLE true;
}

Q_INVOKABLE QVariant TreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->name;

    return QVariant();
}

Qt::ItemFlags TreeItemModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool TreeItemModel::insertRows(int row, int count, const QModelIndex& parent)
{
    auto* parentItem = getItem(parent);

    TreeItem item;
    item.id = QUuid::createUuid();
    item.parent = parentItem;

    //beginInsertRows(parent, row - 1, row + count - 1);
    beginInsertRows(parent, 0, 0);
    parentItem->children.push_back(std::move(item));
    endInsertRows();
    return true;
}

TreeItem* TreeItemModel::getItem(const QModelIndex& idx) const
{
    if (!idx.isValid())
        return rootItem;

    auto* item = static_cast<TreeItem*>(idx.internalPointer());
    return item ? item : rootItem;
}
