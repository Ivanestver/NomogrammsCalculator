#pragma once
#include <QAbstractItemModel>
#include <TreeItem.h>
#include <map>

class TreeItemModel : public QAbstractItemModel
{
	Q_OBJECT;

public:
	enum ModelRole
	{
		NameRole = Qt::UserRole + 1,
		ClassIDRole
	};

public:
	TreeItemModel(const std::vector<STreeItem>& items_, QObject* parent = nullptr);
	~TreeItemModel() override = default;
	TreeItemModel(const TreeItemModel&) = delete;
	TreeItemModel& operator=(const TreeItemModel&) = delete;

	// Унаследовано через QAbstractItemModel
	virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
	virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override;
	virtual Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	virtual Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	bool SaveIndexToDB(const QModelIndex& index, QString& error) const;

private:
	void initItem(STreeItem& itemToInit);
	TreeItem* getItem(const QModelIndex& idx) const;
	bool saveTemplate(const TreeItem* item, QString& error) const;
	bool saveProperties(const TreeItem* item, const std::map<QUuid, QVariant>& properties, QString& error) const;
	bool saveMasterIdForSubId(const TreeItem* item, QString& error) const;

private:
	STreeItem rootItem;
};