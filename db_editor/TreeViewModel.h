#pragma once
#include <QAbstractItemModel>
#include <TreeItem.h>

class TreeViewModel : public QAbstractItemModel
{
	Q_OBJECT;

public:
	TreeViewModel(const std::vector<TreeItem>& items_, QObject* parent = nullptr);
	~TreeViewModel() override = default;

	// Унаследовано через QAbstractItemModel
	virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
	virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override;
	virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override;

private:
	std::vector<TreeItem> items;
};