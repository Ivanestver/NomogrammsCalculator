#include "properties_table_model.h"
#include "db_executor.h"
#include <QDebug>
#include "properties.h"

PropertiesTableModel::PropertiesTableModel(const QUuid& itemId, QObject* parent/* = nullptr*/)
    : QAbstractItemModel(parent)
{
    setIdOfItem(itemId);
    fillPropertiesContainer(itemId);
}

Q_INVOKABLE QModelIndex PropertiesTableModel::index(int row, int column, const QModelIndex& parent) const
{
    if (rowCount(QModelIndex()) <= row)
        return Q_INVOKABLE QModelIndex();

    if (columnCount(QModelIndex()) <= column)
        return Q_INVOKABLE QModelIndex();

    if (column == 0)
        return Q_INVOKABLE createIndex(row, column, (void*)&propertiesContainer.properties[row].name);
    else
        return Q_INVOKABLE createIndex(row, column, (void*)&propertiesContainer.properties[row].value);
}

Q_INVOKABLE QModelIndex PropertiesTableModel::parent(const QModelIndex& child) const
{
    return Q_INVOKABLE QModelIndex();
}

Q_INVOKABLE int PropertiesTableModel::rowCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE (int)propertiesContainer.properties.size();
}

Q_INVOKABLE int PropertiesTableModel::columnCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE 2;
}

Q_INVOKABLE QVariant PropertiesTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return Q_INVOKABLE QVariant();

    if (rowCount(QModelIndex()) <= index.row())
        return Q_INVOKABLE QModelIndex();

    if (columnCount(QModelIndex()) <= index.column())
        return Q_INVOKABLE QModelIndex();

    if (index.column() == 0)
        return Q_INVOKABLE propertiesContainer.properties[index.row()].name;
    else
        return Q_INVOKABLE propertiesContainer.properties[index.row()].value;
}

Q_INVOKABLE bool PropertiesTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return Q_INVOKABLE false;

    auto dbExecutor = DBExecutor::GetInstance();
    if (!dbExecutor)
    {
        qDebug() << QString::fromLocal8Bit("Не был получен экземпляр БД");
        return false;
    }

    auto& propertyInfo = propertiesContainer.properties[index.row()];
    QString error;
    if (!dbExecutor->UpdateProperty(propertiesContainer.id, propertyInfo.id, value, error))
    {
        qDebug() << error;
        return false;
    }

    propertyInfo.value = value;
    nameIsChanged = propertyInfo.id == db_state::properties::dbobject_name;

    return true;
}

Q_INVOKABLE QVariant PropertiesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Orientation::Vertical)
        return Q_INVOKABLE QVariant();

    if (role != Qt::DisplayRole)
        return Q_INVOKABLE QVariant();

    return section == 0 ? QString::fromLocal8Bit("Название свойства") : QString::fromLocal8Bit("Значение свойства");
}

Q_INVOKABLE Qt::ItemFlags PropertiesTableModel::flags(const QModelIndex& index) const
{
    if (index.row() < 2 || index.column() == 0)
        return Q_INVOKABLE QAbstractItemModel::flags(index);
    return Q_INVOKABLE QAbstractItemModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}

bool PropertiesTableModel::NameIsChanged() const
{
    return nameIsChanged;
}

QString PropertiesTableModel::GetChangedName() const
{
    return propertiesContainer.properties[2].value.toString();
}

bool PropertiesTableModel::setIdOfItem(const QUuid& itemId)
{
    auto db = DBExecutor::GetInstance();
    if (!db)
    {
        qDebug() << QString::fromLocal8Bit("Не получен экземпляр БД");
        return false;
    }

    QString error;
    DBExecutor::Response response;
    if (!db->ExecSELECT("select class_id from [template] where [template_id] = ?", { itemId }, response, error))
    {
        qDebug() << error;
        return false;
    }

    if (response.empty() || response[0].empty())
    {
        qDebug() << QString::fromLocal8Bit("Нет такого объекта в БД");
        return false;
    }

    const auto& classIdVariant = response[0].front();

    propertiesContainer.id = itemId;
    propertiesContainer.classId = classIdVariant.toUuid();

    propertiesContainer.properties.push_back(PropertiesTableItem(QUuid(), "ID", itemId));
    propertiesContainer.properties.push_back(PropertiesTableItem(QUuid(), QString::fromLocal8Bit("ID класса"), propertiesContainer.classId));
    return true;
}

bool PropertiesTableModel::fillPropertiesContainer(const QUuid& id)
{
    auto db = DBExecutor::GetInstance();
    if (!db)
        return false;

    std::set<DBExecutor::PropertyInfo> propertiesInfo;
    QString error;
    if (!db->ReceivePropertiesOfObj(id, propertiesInfo, error))
    {
        qDebug() << error;
        return false;
    }

    for (const auto& info : propertiesInfo)
    {
        PropertiesTableItem item;
        item.id = std::get<0>(info);
        item.name = std::get<1>(info);
        item.value = std::get<2>(info);

        propertiesContainer.properties.push_back(item);
    }

    return true;
}