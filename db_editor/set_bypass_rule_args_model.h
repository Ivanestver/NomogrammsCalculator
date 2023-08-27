#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QUuid>
#include <vector>
#include <tuple>

class BypassRuleArgsModel : public QAbstractItemModel
{
	using ItemInfo = std::tuple<QUuid, QString, QString>;

public:
	BypassRuleArgsModel(const QUuid& nomogrammId_, QObject* parent = nullptr);
	~BypassRuleArgsModel() override = default;

	// Унаследовано через QAbstractItemModel
	virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
	virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override;
	virtual Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex& index) const;

private:
	void fetchSubs();

private:
	QUuid nomogrammId;
	std::vector<ItemInfo> items;
};