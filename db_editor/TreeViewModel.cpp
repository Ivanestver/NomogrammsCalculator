#include "TreeViewModel.h"
#include "db_executor.h"
#include <QDebug>
#include "db_state/properties.h"

TreeItemModel::TreeItemModel(const std::vector<STreeItem>& items_, QObject* parent/* = nullptr*/)
    : QAbstractItemModel(parent)
{
    rootItem = std::make_shared<TreeItem>(nullptr);
    rootItem->children.insert(rootItem->children.begin(), items_.begin(), items_.end());
    initItem(rootItem);
}

Q_INVOKABLE QModelIndex TreeItemModel::index(int row, int column, const QModelIndex& parent) const
{
    auto* parentPtr = getItem(parent);
    if (!parentPtr
      || parentPtr->children.empty()
      || parentPtr->children.size() <= (size_t)row)
        return Q_INVOKABLE QModelIndex();

    return Q_INVOKABLE createIndex(row, column, (void*)parentPtr->children[row].get());
}

Q_INVOKABLE QModelIndex TreeItemModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return Q_INVOKABLE QModelIndex();

    try
    {
        const auto* childPtr = getItem(child);
        const auto* parentPtr = childPtr->parent;
        if (!parentPtr || *parentPtr == *rootItem)
            return Q_INVOKABLE QModelIndex();

        const auto it = std::find_if(parentPtr->children.begin(), parentPtr->children.end(), [childPtr](const STreeItem& item)
            {
                return *childPtr == *item;
            });
        if (it == parentPtr->children.end())
            return Q_INVOKABLE QModelIndex();

        return Q_INVOKABLE createIndex(it - parentPtr->children.begin(), 0, (void*)parentPtr);
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
        return Q_INVOKABLE QModelIndex();
    }
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

    auto item = std::make_shared<TreeItem>(nullptr);
    item->id = QUuid::createUuid();
    item->parent = parentItem;

    beginInsertRows(parent, row, row + count - 1);
    parentItem->children.push_back(std::move(item));
    endInsertRows();

    return true;
}

bool TreeItemModel::removeRows(int row, int count, const QModelIndex& parent)
{
    auto* parentItem = getItem(parent);

    beginRemoveRows(parent, row, row + count - 1);
    parentItem->children.erase(parentItem->children.begin() + row);
    endRemoveRows();

    return true;
}

bool TreeItemModel::SaveIndexToDB(const QModelIndex& index, QString& error) const
{
    if (!index.isValid())
        return false;

    const auto* item = getItem(index);
    if (!item)
        return false;

    if (!saveTemplate(item, error))
        return false;
    if (!saveProperties(item, { {db_state::properties::dbobject_name, item->name} }, error))
        return false;

    saveMasterIdForSubId(item, error);
    
    return true;
}

void TreeItemModel::initItem(STreeItem& itemToInit)
{
    for (auto& item : itemToInit->children)
    {
        item->parent = itemToInit.get();

        auto executor = DBExecutor::GetInstance();
        std::vector<std::vector<QVariant>> results;
        QString queryStr = "select t1.sub_id, class_id, property_value\
 from(template_template as t1\
 inner join template_property as t2\
 on t1.sub_id = t2.template_id)\
 inner join template as t3\
 on t1.sub_id = t3.template_id\
 where master_id = ?\
 and property_id = ?";
        QString error = executor->ExecSELECT(queryStr, { DBExecutor::DBExecutorUtils::TurnUuidToStr(item->id), DBExecutor::DBExecutorUtils::TurnUuidToStr(db_state::properties::dbobject_name) }, results);
        if (error.isEmpty())
        {
            for (const auto& result : results)
            {
                auto newTreeItem = std::make_shared<TreeItem>(nullptr);
                newTreeItem->id = result[0].toUuid();
                newTreeItem->classId = result[1].toUuid();
                newTreeItem->name = result[2].toString();
                newTreeItem->parent = item.get();

                item->children.push_back(newTreeItem);
            }
        }
        else
        {
            qDebug() << error;
        }

        initItem(item);
    }
}

TreeItem* TreeItemModel::getItem(const QModelIndex& idx) const
{
    if (!idx.isValid())
        return rootItem.get();

    auto* item = static_cast<TreeItem*>(idx.internalPointer());
    return item ? item : rootItem.get();
}

bool TreeItemModel::saveTemplate(const TreeItem* item, QString& error) const
{
    if (!item
      || item->id.isNull()
      || item->classId.isNull())
        return false;

    error = "";

    auto executor = DBExecutor::GetInstance();
    int rowsInserted = executor->ExecChange("insert into [template](template_id, class_id) values (?, ?)", { item->id, item->classId }, error);
    return rowsInserted != 0;
}

bool TreeItemModel::saveProperties(const TreeItem* item, const std::map<QUuid, QVariant>& properties, QString& error) const
{
    if (!item
     || item->id.isNull()
     || properties.empty())
        return false;

    error = "";

    auto executor = DBExecutor::GetInstance();
    QString queryStr = "insert into [template_property](template_id, property_id, property_value) values (?, ?, ?)";
    size_t rowsAffected = 0;
    for (const auto& pair : properties)
    {
        rowsAffected += executor->ExecChange(queryStr, { item->id, pair.first, pair.second }, error);
    }

    return rowsAffected == properties.size();
}

bool TreeItemModel::saveMasterIdForSubId(const TreeItem* item, QString& error) const
{
    if (!item
     || item->id.isNull()
     || !item->parent
     || item->parent->id.isNull())
        return false;

    error = "";

    if (*item->parent == *rootItem)
        return true;

    auto executor = DBExecutor::GetInstance();
    int rowsAffected = executor->ExecChange("insert into [template_template](master_id, sub_id) values (?, ?)", { item->parent->id, item->id }, error);

    return rowsAffected != 0;
}