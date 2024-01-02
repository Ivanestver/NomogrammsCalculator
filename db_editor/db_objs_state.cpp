#include "db_objs_state.h"
#include <QInputDialog>
#include "TreeViewModel.h"
#include "db_state/properties.h"
#include "choose_item_type_dlg.h"
#include "choose_nn.h"

AbstractDBObjState::AbstractDBObjState()
{
	propertiesIds.insert(db_state::properties::dbobject_name);
}

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

const std::set<QUuid>& AbstractDBObjState::GetPropertiesIds() const
{
	return propertiesIds;
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

const QUuid& AbstractDBObjState::getCustomItemId() const
{
	return QUuid();
}

void AbstractDBObjState::addProperty(const QUuid& propertyId)
{
	propertiesIds.insert(propertyId);
}

void AbstractDBObjState::setItemToAdd(TreeItem* itemToAdd_)
{
	this->itemToAdd = itemToAdd_;
}

QString AbstractDBObjState::openInputNameDlgAndGetDlg()
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

		const QUuid& customId = getCustomItemId();
		if (!customId.isNull())
			model->setData(added, customId, TreeItemModel::ModelRole::ItemIDRole);

		return static_cast<TreeItem*>(added.internalPointer());
	}

	error = QString::fromLocal8Bit("Ошибка при добавлении атрибутов новой методики");
	return nullptr;
}

bool AbstractDBObjState::addObjAndAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	int rowsInserted = executor->ExecChange("insert into template(template_id, class_id) values (?, ?)", { item->id, item->classId}, error);
	if (rowsInserted < 0)
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

	if (!removeItemFromTable(itemId, "template", "template_id", error))
		return false;

	return commitAdditionalDeletes(itemId, error);
}

bool AbstractDBObjState::removeItemFromTable(const QUuid& itemId, const QString& tableName, const QString& columnName, QString& error) const
{
	auto dbExecutor = DBExecutor::GetInstance();
	if (!dbExecutor)
		return false;

	QString queryStr = QString("delete from %1 where %2 = ?").arg(tableName).arg(columnName);
	return dbExecutor->ExecChange(queryStr, { itemId }, error) > 0;
}

bool AbstractDBObjState::commitAdditionalDeletes(const QUuid& itemId, QString& error) const
{
	Q_UNUSED(itemId);
	return true;
}

bool MethodologyState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	return executor->InsertProperty(item->id, db_state::properties::dbobject_name, item->name, error);
}

QUuid MethodologyState::getClassId() const
{
	return MethodologyState::GetClassID();
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

QUuid MethodologyState::GetClassID()
{
	return methodology_class;
}

void MethodologyState::FillProperties()
{
}

QUuid NomogrammState::GetClassID()
{
	return nomogramm_class;
}

QUuid NomogrammState::getClassId() const
{
	return NomogrammState::GetClassID();
}

QModelIndex NomogrammState::getParentIndex(const QModelIndexList& selectedIndexList) const
{
	return !selectedIndexList.isEmpty() ? selectedIndexList.front() : QModelIndex();
}

bool NomogrammState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	if (!executor->LinkTemplates(item->parent->id, item->id, error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::dbobject_name, item->name, error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::nomogramm_bypassRule, QString(""), error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::nomogramm_bypassRule_List, QString(""), error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::data_type_with_unit, QString(""), error))
		return false;

	return addPictureToDB(executor, item->id, error);
}

std::pair<QString, QString> NomogrammState::getMessageAndTitleWhenAdding() const
{
	return { QString::fromLocal8Bit("Введите название номограммы") , QString::fromLocal8Bit("Название номограммы") };
}

void NomogrammState::FillProperties()
{
	addProperty(db_state::properties::nomogramm_bypassRule);
	addProperty(db_state::properties::nomogramm_bypassRule_List);
	addProperty(db_state::properties::data_type_with_unit);
	addProperty(db_state::properties::nomogramm_picture);
}

bool NomogrammState::addPictureToDB(const std::shared_ptr<DBExecutor>& executor, const QUuid& itemId, QString& error) const
{
	if (!executor->InsertProperty(itemId, db_state::properties::nomogramm_picture, QString(""), error))
		return false;

	return executor->ExecChange("insert into nomogramms_images(template_id) values (?)", { itemId }, error) > 0;
}

QUuid GraphicState::getClassId() const
{
	return GraphicState::GetClassID();
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

	if (!executor->InsertProperty(item->id, db_state::properties::data_type_with_unit, item->name, error))
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::data_output_type_with_unit, item->name, error))
		return false;

	return executor->LinkTemplates(item->parent->id, item->id, error);
}

std::pair<QString, QString> GraphicState::getMessageAndTitleWhenAdding() const
{
	return { QString::fromLocal8Bit("Введите название графика") , QString::fromLocal8Bit("Название графика") };
}

bool GraphicState::commitAdditionalDeletes(const QUuid& itemId, QString& error) const
{
	if (!removeItemFromTable(itemId, "template_measure_unit_input", "template_id", error))
		return false;

	return removeItemFromTable(itemId, "template_measure_unit_output", "template_id", error);
}

QUuid GraphicState::GetClassID()
{
	return graphics_class;
}

void GraphicState::FillProperties()
{
	addProperty(db_state::properties::dbobject_name);
	addProperty(db_state::properties::data_type_with_unit);
	addProperty(db_state::properties::data_output_type_with_unit);
}

std::shared_ptr<AbstractDBObjState> NomogrammGraphicStateCreator::CreateObj() const
{
	DlgChooseItemType dlg;
	if (dlg.exec() == QDialog::Accepted)
		return dlg.IsNomogramm() ? StateCreator<NomogrammState>().CreateObj() : StateCreator<GraphicState>().CreateObj();

	return nullptr;
}

QUuid NNState::GetClassID()
{
	return net_class;
}

void NNState::FillProperties()
{
	addProperty(db_state::properties::dbobject_name);
}

QUuid NNState::getClassId() const
{
	return NNState::GetClassID();
}

QModelIndex NNState::getParentIndex(const QModelIndexList& selectedIndexList) const
{
	return selectedIndexList.isEmpty() ? QModelIndex() : selectedIndexList.first();
}

bool NNState::addAttrsToDB(const std::shared_ptr<DBExecutor>& executor, QString& error) const
{
	const auto* item = getItemToAdd();
	if (!item)
		return false;

	if (!executor->InsertProperty(item->id, db_state::properties::dbobject_name, getObjectName(), error))
		return false;

	return executor->LinkTemplates(item->parent->id, item->id, error);
}

std::pair<QString, QString> NNState::getMessageAndTitleWhenAdding() const
{
	return std::pair<QString, QString>();
}

QString NNState::openInputNameDlgAndGetDlg()
{
	DlgChooseNN dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		auto [nnName, nnId] = dlg.GetChosenItemInfo();
		m_id = nnId;
		return nnName;
	}

	return QString();
}

const QUuid& NNState::getCustomItemId() const
{
	return m_id;
}

std::shared_ptr<AbstractDBObjState> NNStateCreator::CreateObj() const
{
	return StateCreator<NNState>().CreateObj();
}
