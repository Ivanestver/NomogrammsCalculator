#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QList>
#include "db_executor.h"
#include "TreeItem.h"

class AbstractDBObjState
{
public:
	AbstractDBObjState() = default;
	virtual ~AbstractDBObjState() = default;

	bool AddNewObjToModelAndThenToDB(QAbstractItemModel* abstractModel, const QModelIndexList& selectedIndexList, QString& error);
	bool RemoveItem(const QModelIndex& selectedItemIdx, QAbstractItemModel* abstractModel, QString& error) const;

protected:
	virtual QUuid getClassId() const = 0;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const = 0;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const = 0;

protected:
	const QString& getObjectName() const;
	const TreeItem* getItemToAdd() const;

private:
	void setObjectName(const QString& objectName_);
	void setItemToAdd(TreeItem* itemToAdd_);
	virtual QString openInputNameDlgAndGetDlg() const;
	// Вспомогательные функции для добавления
	TreeItem* insertRowToModelAndGetItem(QAbstractItemModel* model, const QModelIndexList& selectedIndexList, QString& error);
	bool addObjAndAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const;
	// Вспомогательные функции для удаления
	bool removeItemFromModel(const QModelIndex& selectedItemIdx, QAbstractItemModel* model, QString& error) const;
	bool removeItemFromDatabase(const QUuid& itemId, QString& error) const;
	bool removeItemFromTable(const QUuid& itemId, const QString& tableName, const QString& columnName, QString& error) const;

private:
	QString objectName;
	const TreeItem* itemToAdd = nullptr;
};

class MethodologyState : public AbstractDBObjState
{
public:
	MethodologyState() = default;
	~MethodologyState() override = default;

private:
	// Унаследовано через AbstractDBObjState
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
};

class NomogrammState : public AbstractDBObjState
{
public:
	NomogrammState() = default;
	~NomogrammState() override = default;

	static QUuid GetClassID();

private:
	// Унаследовано через AbstractDBObjState
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
};

class GraphicState : public AbstractDBObjState
{
public:
	GraphicState() = default;
	~GraphicState() override = default;

private:

	// Унаследовано через AbstractDBObjState
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
};

class IStateCreator
{
public:
	virtual ~IStateCreator() = default;
	virtual std::shared_ptr<AbstractDBObjState> CreateObj() const = 0;
};

template<class T>
class StateCreator : public IStateCreator
{
public:
	~StateCreator() override = default;

	std::shared_ptr<AbstractDBObjState> CreateObj() const override
	{
		return std::make_shared<T>();
	}
};

class NomogrammGraphicStateCreator : public IStateCreator
{
public:
	~NomogrammGraphicStateCreator() override = default;

	// Унаследовано через IStateCreator
	virtual std::shared_ptr<AbstractDBObjState> CreateObj() const override;
};