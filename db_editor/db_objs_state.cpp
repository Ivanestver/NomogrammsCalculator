#include "db_objs_state.h"
#include <QInputDialog>
#include "TreeViewModel.h"
#include "db_state/properties.h"
#include "choose_item_type_dlg.h"

const QUuid methodology_class("A8A4951D-8542-4CFA-B593-ECBA3DE727D1");
const QUuid nomogramm_class("F5313633-C8FC-43DC-A92E-88B7EE8DF439");
const QUuid rule_class("BB75693A-4506-47DE-9DE6-6F40C8BC3C74");
const QUuid graphics_class("6DCE13AE-8897-48EC-9B4E-664845D40D73");

bool AbstractDBObjState::AddNewObjToModelAndThenToDB(QAbstractItemModel* abstractModel, const QModelIndexList& selectedIndexList, QString& error)
{
	if (!abstractModel)
	{
		error = QString::fromLocal8Bit("Получена невалидная модель");
		return false;
	}

	auto objName = openInputNameDlgAndGetDlg();
	if (objName.isEmpty())
	{
		error = QString::fromLocal8Bit("Название не может быть пустым!");
		return false;
	}

	setObjectName(objName);

	auto* item = insertRowToModelAndGetItem(abstractModel, selectedIndexList, error);
	if (!item)
		return false;

	setItemToAdd(item);

	auto db = DBExecutor::GetInstance();
	return addObjAndAttrsToDB(db, error);
}

bool AbstractDBObjState::RemoveItem(const QModelIndex& selectedItemIdx, QAbstractItemModel* abstractModel, QString& error) const
{
	if (!selectedItemIdx.isValid())
	{
		error = QString::fromLocal8Bit("Не выбран элемент");
		return false;
	}

	if (!abstractModel)
	{
		error = QString::fromLocal8Bit("Модель невалидна");
		return false;
	}

	const auto* itemToRemove = static_cast<const TreeItem*>(selectedItemIdx.internalPointer());
	QUuid itemId = itemToRemove->id;

	if (!removeItemFromModel(selectedItemIdx, abstractModel, error))
		return false;

	return removeItemFromDatabase(itemId, error);
}

const QString& AbstractDBObjState::getObjectName() const
{
	return objectName;
}

void AbstractDBObjState::setObjectName(const QString& objectName_)
{
	this->objectName = objectName_;
}

const TreeItem* AbstractDBObjState::getItemToAdd() const
{
	return itemToAdd;
}

void AbstractDBObjState::setItemToAdd(TreeItem* itemToAdd_)
{
	this->itemToAdd = itemToAdd_;
}

QString AbstractDBObjState::openInputNameDlgAndGetDlg() const
{
	const auto messageAndTitle = getMessageAndTitleWhenAdding();
	return QInputDialog::getText(nullptr, messageAndTitle.first, messageAndTitle.second);
}

TreeItem* AbstractDBObjState::insertRowToModelAndGetItem(QAbstractItemModel* model, const QModelIndexList& selectedIndexList, QString& error)
{
	auto modelIndex = getParentIndex(selectedIndexList);
	if (!model->insertRow(model->rowCount(modelIndex), modelIndex))
	{
		error = QString::fromLocal8Bit("Ошибка при добавлении новой методики");
		return nullptr;
	}

	const auto added = model->index(model->rowCount(modelIndex) - 1, 1, modelIndex);
	if (added.isValid())
	{
		model->setData(added, getObjectName(), TreeItemModel::ModelRole::NameRole);
		model->setData(added, getClassId(), TreeItemModel::ModelRole::ClassIDRole);

		return static_cast<TreeItem*>(added.internalPointer());
	}

	error = QString::fromLocal8Bit("Ошибка при добавлении атрибутов новой методики");
	return nullptr;
}

bool AbstractDBObjState::addObjAndAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	int rowsInserted = executor->ExecChange("insert into [template](template_id, class_id) values (?, ?)", { item->id, item->classId }, error);
	if (rowsInserted == 0)
		return false;

	return addAttrsToDB(executor, error);
}

bool AbstractDBObjState::removeItemFromModel(const QModelIndex& selectedItemIdx, QAbstractItemModel* model, QString& error) const
{
	Q_UNUSED(error);
	return model->removeRow(selectedItemIdx.row(), selectedItemIdx.parent());
}

bool AbstractDBObjState::removeItemFromDatabase(const QUuid& itemId, QString& error) const
{
	removeItemFromTable(itemId, "template_template", "master_id", error);
	if (!error.isEmpty())
		return false;

	removeItemFromTable(itemId, "template_template", "sub_id", error);
	if (!error.isEmpty())
		return false;

	if (!removeItemFromTable(itemId, "template_property", "template_id", error))
		return false;

	return removeItemFromTable(itemId, "template", "template_id", error);
}

bool AbstractDBObjState::removeItemFromTable(const QUuid& itemId, const QString& tableName, const QString& columnName, QString& error) const
{
	auto dbExecutor = DBExecutor::GetInstance();
	if (!dbExecutor)
		return false;

	QString queryStr = QString("delete from [%1] where [%2] = ?").arg(tableName).arg(columnName);
	return dbExecutor->ExecChange(queryStr, { itemId }, error) > 0;
}

bool MethodologyState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	return executor->InsertProperty(item->id, db_state::properties::dbobject_name, item->name, error);
}

QUuid MethodologyState::getClassId() const
{
	return methodology_class;
}

QModelIndex MethodologyState::getParentIndex(const QModelIndexList& selectedIndexList) const
{
	Q_UNUSED(selectedIndexList);
	return QModelIndex();
}

std::pair<QString, QString> MethodologyState::getMessageAndTitleWhenAdding() const
{
	return { QString::fromLocal8Bit("Введите название методики") , QString::fromLocal8Bit("Название методики") };
}

QUuid NomogrammState::GetClassID()
{
	return NomogrammState().getClassId();
}

QUuid NomogrammState::getClassId() const
{
	return nomogramm_class;
}

QModelIndex NomogrammState::getParentIndex(const QModelIndexList& selectedIndexList) const
{
	return !selectedIndexList.isEmpty() ? selectedIndexList.front() : QModelIndex();
}

bool NomogrammState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	if (!executor->InsertProperty(item->id, db_state::properties::dbobject_name, item->name, error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::nomogramm_bypassRule, QString(""), error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::nomogramm_bypassRule_List, QString(""), error))
		return false; 

	if (!executor->InsertProperty(item->id, db_state::properties::data_type_with_unit, QString(""), error))
		return false;

	return executor->LinkTemplates(item->parent->id, item->id, error);
}

std::pair<QString, QString> NomogrammState::getMessageAndTitleWhenAdding() const
{
	return { QString::fromLocal8Bit("Введите название номограммы") , QString::fromLocal8Bit("Название номограммы") };
}

QUuid GraphicState::getClassId() const
{
	return graphics_class;
}

QModelIndex GraphicState::getParentIndex(const QModelIndexList& selectedIndexList) const
{
	return !selectedIndexList.isEmpty() ? selectedIndexList.front() : QModelIndex();
}

bool GraphicState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	if (!executor->InsertProperty(item->id, db_state::properties::dbobject_name, item->name, error))
		return false;

	return executor->LinkTemplates(item->parent->id, item->id, error);
}

std::pair<QString, QString> GraphicState::getMessageAndTitleWhenAdding() const
{
	return { QString::fromLocal8Bit("Введите название графика") , QString::fromLocal8Bit("Название графика") };
}

std::shared_ptr<AbstractDBObjState> NomogrammGraphicStateCreator::CreateObj() const
{
	DlgChooseItemType dlg;
	if (dlg.exec() == QDialog::Accepted)
		return dlg.IsNomogramm() ? StateCreator<NomogrammState>().CreateObj() : StateCreator<GraphicState>().CreateObj();

	return std::shared_ptr<AbstractDBObjState>();
}