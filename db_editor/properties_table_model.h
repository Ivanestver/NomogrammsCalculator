#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QUuid>
#include "properties_table_item.h"

class PropertiesTableModel : public QAbstractItemModel
{
public:
	PropertiesTableModel(const QUuid& itemId, QObject* parent = nullptr);
	~PropertiesTableModel() override = default;

	// Унаследовано через QAbstractItemModel
	virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
	virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override;
	virtual Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	virtual Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex& index) const;

	bool NameIsChanged() const;
	QString GetChangedName() const;
	const PropertiesTableItem& GetTableItemByIdx(const QModelIndex& index) const;

private:
	bool setIdOfItem(const QUuid& itemId);
	bool fillPropertiesContainer(const QUuid& id);

private:
	PropertiesTableItemInfo propertiesContainer;
	bool nameIsChanged = false;
	QString measureUnitTableName;
};