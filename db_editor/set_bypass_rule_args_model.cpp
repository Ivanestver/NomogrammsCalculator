#include "set_bypass_rule_args_model.h"
#include "db_executor.h"
#include <QDebug>
#include "properties.h"

BypassRuleArgsModel::BypassRuleArgsModel(const QUuid& nomogrammId_, QObject* parent)
    : QAbstractItemModel(parent)
    , nomogrammId(nomogrammId_)
{
    fetchSubs();
}

Q_INVOKABLE QModelIndex BypassRuleArgsModel::index(int row, int column, const QModelIndex& parent) const
{
    if (items.size() <= row)
        return Q_INVOKABLE QModelIndex();
    return Q_INVOKABLE createIndex(row, column, (void*)&(column == 0 ? std::get<1>(items[row]) : std::get<2>(items[row])));
}

Q_INVOKABLE QModelIndex BypassRuleArgsModel::parent(const QModelIndex& child) const
{
    return Q_INVOKABLE QModelIndex();
}

Q_INVOKABLE int BypassRuleArgsModel::rowCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE (int)items.size();
}

Q_INVOKABLE int BypassRuleArgsModel::columnCount(const QModelIndex& parent) const
{
    return Q_INVOKABLE 2;
}

Q_INVOKABLE QVariant BypassRuleArgsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return Q_INVOKABLE QVariant();

    return Q_INVOKABLE (index.column() == 0 ? std::get<1>(items[index.row()]) : std::get<2>(items[index.row()]));
}

Q_INVOKABLE bool BypassRuleArgsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return Q_INVOKABLE false;

    auto db = DBExecutor::GetInstance();
    if (!db)
        return Q_INVOKABLE false;

    auto& itemInfo = items[index.row()];
    QString error;
    if (!db->ExecChange("update [template_template] set [extra] = ? where [master_id] = ? and [sub_id] = ?", { value.toString(), nomogrammId, std::get<0>(itemInfo)}, error))
    {
        qDebug() << error;
        return Q_INVOKABLE false;
    }

    std::get<2>(itemInfo) = value.toString();

    return Q_INVOKABLE true;
}

Q_INVOKABLE QVariant BypassRuleArgsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return Q_INVOKABLE QVariant();

    return Q_INVOKABLE QString::fromLocal8Bit(section == 0 ? "Название подчинённого графика" : "Имя переменной");
}

Q_INVOKABLE Qt::ItemFlags BypassRuleArgsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid() || index.column() == 0)
        return Q_INVOKABLE QAbstractItemModel::flags(index);
    return Q_INVOKABLE QAbstractItemModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}

void BypassRuleArgsModel::fetchSubs()
{
    auto db = DBExecutor::GetInstance();
    if (!db)
        return;

    DBExecutor::Response response;
    QString error;
    QString queryStr = "select t1.sub_id, t2.property_value, t1.extra from [template_template] as t1 inner join [template_property] as t2 on t1.sub_id=t2.template_id where [master_id] = ? and [property_id] = ?";
    if (!db->ExecSELECT(queryStr, { nomogrammId, db_state::properties::dbobject_name }, response, error))
    {
        qDebug() << error;
        return;
    }

    for (const auto& record : response)
    {
        items.push_back({ record[0].toUuid(), record[1].toString(), record[2].toString() });
    }
}