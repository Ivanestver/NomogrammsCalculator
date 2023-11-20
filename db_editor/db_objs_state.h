#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QList>
#include "db_executor.h"
#include "TreeItem.h"
#include <set>

const QUuid methodology_class("A8A4951D-8542-4CFA-B593-ECBA3DE727D1");
const QUuid nomogramm_class("F5313633-C8FC-43DC-A92E-88B7EE8DF439");
const QUuid rule_class("BB75693A-4506-47DE-9DE6-6F40C8BC3C74");
const QUuid graphics_class("6DCE13AE-8897-48EC-9B4E-664845D40D73");
const QUuid net_class("DD98B7A5-B5A0-4EBF-890A-22F20805651D");

class MethodologyState;
class NomogrammState;
class GraphicState;
class NetState;

class AbstractDBObjState
{
public:
	template<class T>
	static std::shared_ptr<AbstractDBObjState> CreateState()
	{
		static_assert(std::is_base_of<AbstractDBObjState, T>::value, "T должен быть наследником класса AbstractDBObjState");
		auto ptr = std::make_shared<T>();
		ptr->FillProperties();
		return ptr;
	}

	static std::shared_ptr<AbstractDBObjState> CreateStateById(const QUuid& classId)
	{
		std::shared_ptr<AbstractDBObjState> ptr;
		if (classId == methodology_class)
			ptr = AbstractDBObjState::CreateState<MethodologyState>();
		else if (classId == nomogramm_class)
			ptr = AbstractDBObjState::CreateState<NomogrammState>();
		else if (classId == graphics_class)
			ptr = AbstractDBObjState::CreateState<GraphicState>();
		else
			return ptr;

		ptr->FillProperties();
		return ptr;
	}

	virtual ~AbstractDBObjState() = default;

	bool AddNewObjToModelAndThenToDB(QAbstractItemModel* abstractModel, const QModelIndexList& selectedIndexList, QString& error);
	bool RemoveItem(const QModelIndex& selectedItemIdx, QAbstractItemModel* abstractModel, QString& error) const;
	const std::set<QUuid>& GetPropertiesIds() const;
	virtual void FillProperties() = 0;

protected:
	AbstractDBObjState();
	virtual QUuid getClassId() const = 0;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const = 0;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const = 0;
	virtual std::pair<QString, QString> getMessageAndTitleWhenAdding() const = 0;

protected:
	const QString& getObjectName() const;
	const TreeItem* getItemToAdd() const;
	void addProperty(const QUuid& propertyId);

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
	std::set<QUuid> propertiesIds;
};

class MethodologyState : public AbstractDBObjState
{
public:
	static QUuid GetClassID();

	MethodologyState() = default;
	~MethodologyState() override = default;

	virtual void FillProperties() override;
private:
	// Унаследовано через AbstractDBObjState
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
	virtual std::pair<QString, QString> getMessageAndTitleWhenAdding() const override;
};

class NomogrammState : public AbstractDBObjState
{
public:
	NomogrammState() = default;
	~NomogrammState() override = default;

	static QUuid GetClassID();
	virtual void FillProperties() override;

private:
	// Унаследовано через AbstractDBObjState
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
	virtual std::pair<QString, QString> getMessageAndTitleWhenAdding() const override;
};

class GraphicState : public AbstractDBObjState
{
public:
	GraphicState() = default;
	~GraphicState() override = default;
	
	static QUuid GetClassID();
	virtual void FillProperties() override;
private:
	// Унаследовано через AbstractDBObjState
	virtual QUuid getClassId() const override;
	virtual QModelIndex getParentIndex(const QModelIndexList& selectedIndexList) const override;
	virtual bool addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const override;
	virtual std::pair<QString, QString> getMessageAndTitleWhenAdding() const override;
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
		return AbstractDBObjState::CreateState<T>();
	}
};

class NomogrammGraphicStateCreator : public IStateCreator
{
public:
	~NomogrammGraphicStateCreator() override = default;

	// Унаследовано через IStateCreator
	virtual std::shared_ptr<AbstractDBObjState> CreateObj() const override;
};