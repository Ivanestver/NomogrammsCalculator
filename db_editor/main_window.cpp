#include "main_window.h"
#include "db_executor.h"
#include <QToolBar>
#include <QUuid>
#include "TreeViewModel.h"
#include "db_state/properties.h"
#include <QInputDialog>
#include "choose_item_type_dlg.h"
#include "obj_properties_dlg.h"
#include <QDebug>
#include "measures_list_dlg.h"

MainWindow::MainWindow(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	initTree();
	addToolBar();
	addContextMenuToTree();

	factoryMap.insert({ methodology_class, std::make_shared<StateCreator<MethodologyState>>()});
	factoryMap.insert({ nomogramm_class, std::make_shared<NomogrammGraphicStateCreator>()});
	factoryMap.insert({ graphics_class, std::make_shared<NomogrammGraphicStateCreator>()});
	factoryMap.insert({ net_class, std::make_shared<StateCreator<NNState>>()});

	factoryMapIndependent.insert({ methodology_class, std::make_shared<StateCreator<MethodologyState>>()});
	factoryMapIndependent.insert({ nomogramm_class, std::make_shared<StateCreator<NomogrammState>>()});
	factoryMapIndependent.insert({ graphics_class, std::make_shared<StateCreator<GraphicState>>()});
	factoryMapIndependent.insert({ net_class, std::make_shared<StateCreator<NNState>>()});

	mappingRules.insert({QUuid(), methodology_class});
	mappingRules.insert({methodology_class, nomogramm_class});
	mappingRules.insert({ nomogramm_class, nomogramm_class });
	mappingRules.insert({ graphics_class, net_class });
}

void MainWindow::initTree()
{
	auto dbExecutor = DBExecutor::GetInstance();
	if (!dbExecutor)
		return;

	QString queryStr = "SELECT t1.template_id as template_id, property_value\
 from template as t1\
 inner join template_property as t2 on t1.template_id = t2.template_id\
 where class_id = ? \
 and property_id = ?";

	std::vector<std::vector<QVariant>> results;
	QString error;
	if (!dbExecutor->ExecSELECT(queryStr, { DBExecutor::DBExecutorUtils::TurnUuidToStr(methodology_class), DBExecutor::DBExecutorUtils::TurnUuidToStr(db_state::properties::dbobject_name) }, results, error))
	{
		showWarning(error);
		return;
	}

	std::vector<STreeItem> ids;
	for (const auto& result : results)
	{
		auto item = std::make_shared<TreeItem>();
		item->id = result[0].toUuid();
		item->name = result[1].toString();
		item->classId = methodology_class;
		ids.push_back(item);
	}

	m_ui.treeView->setModel(new TreeItemModel(ids));
	m_ui.treeView->selectAll();
}

void MainWindow::addToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новый объект"), this, &MainWindow::onAddItem);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новую методику"), this, &MainWindow::onAddMethodology);
	toolBar->addAction(QString::fromLocal8Bit("Удалить выбранный объект"), this, &MainWindow::onRemoveItem);
	toolBar->addAction(QString::fromLocal8Bit("Открыть типы данных"), this, &MainWindow::onOpenTypesWindowBtnClicked);
}

void MainWindow::addContextMenuToTree()
{
	m_ui.treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(m_ui.treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onCustomMenuRequested);

	treeContextMenu = new QMenu(m_ui.treeView);
	treeContextMenu->addAction(QString::fromLocal8Bit("Удалить выбранный объект"), this, &MainWindow::onRemoveItem);
	treeContextMenu->addAction(QString::fromLocal8Bit("Свойства"), this, &MainWindow::onPropertiesMenuActionClicked);
}

QMessageBox::StandardButton MainWindow::showWarning(const QString& message)
{
	return QMessageBox::warning(this, QString::fromLocal8Bit("Ошибка"), message);
}

void MainWindow::onAddMethodology()
{
	auto* model = dynamic_cast<TreeItemModel*>(m_ui.treeView->model());
	if (!model)
	{
		showWarning(QString::fromLocal8Bit("Ошибка при получении модели дерева"));
		return;
	}

	const auto& creator = factoryMap.find(methodology_class)->second;
	auto dbObjState = creator->CreateObj();
	QString error;
	if (!dbObjState->AddNewObjToModelAndThenToDB(model, QModelIndexList(), error))
		showWarning(error);
}

void MainWindow::onRemoveItem()
{
	const auto selectedIdxs = m_ui.treeView->selectionModel()->selectedIndexes();
	if (selectedIdxs.isEmpty())
	{
		showWarning(QString::fromLocal8Bit("Выберите объект для удаления"));
		return;
	}

	const auto& selected = selectedIdxs.first();
	const auto* itemToRemove = static_cast<const TreeItem*>(selected.internalPointer());
	auto itStateFactory = factoryMapIndependent.find(itemToRemove->classId);
	if (itStateFactory == factoryMapIndependent.end())
	{
		showWarning(QString::fromLocal8Bit("Незарегистрированный тип объекта"));
		return;
	}

	auto state = itStateFactory->second->CreateObj();
	if (!state)
	{
		showWarning(QString::fromLocal8Bit("Не было создано состояние"));
		return;
	}

	QString error;
	if (!state->RemoveItem(selected, m_ui.treeView->model(), error))
		showWarning(error);
}

void MainWindow::onCustomMenuRequested(const QPoint& point)
{
	QModelIndex index = m_ui.treeView->indexAt(point);
	if (index.isValid()) {
		treeContextMenu->exec(m_ui.treeView->viewport()->mapToGlobal(point));
	}
}

void MainWindow::onPropertiesMenuActionClicked()
{
	const auto selectedIdxs = m_ui.treeView->selectionModel()->selectedIndexes();
	assert(!selectedIdxs.isEmpty());

	const auto& selectedItemIdx = selectedIdxs.first();
	auto* item = static_cast<TreeItem*>(selectedItemIdx.internalPointer());

	DlgObjProperties dlg(item->id, this);
	dlg.exec();
	if (dlg.NameIsChanged())
		item->name = dlg.GetChangedName();
}

void MainWindow::onOpenTypesWindowBtnClicked()
{
	DlgMeasuresUnitsList dlg(this);
	dlg.exec();
}

void MainWindow::onAddItem()
{
	auto* model = m_ui.treeView->model();
	if (!model)
	{
		showWarning(QString::fromLocal8Bit("Ошибка при получении модели дерева"));
		return;
	}

	const auto selectedIdxs = m_ui.treeView->selectionModel()->selectedIndexes();
	if (selectedIdxs.isEmpty())
	{
		showWarning(QString::fromLocal8Bit("Выберите родительский элемент!"));
		return;
	}

	const auto& selected = selectedIdxs.first();
	const auto* selectedItem = static_cast<const TreeItem*>(selected.internalPointer());
	if (!selectedItem)
	{
		showWarning(QString::fromLocal8Bit("Нет данных в выбранной ячейке"));
		return;
	}

	if (selectedItem->classId == net_class)
	{
		return;
	}

	auto it = mappingRules.find(selectedItem->classId);
	if (it == mappingRules.end())
	{
		showWarning(QString::fromLocal8Bit("Нет правила отражения для класса с ID = %1").arg(selectedItem->classId.toString()));
		return;
	}

	const auto itFactory = factoryMap.find(it->second);
	if (itFactory == factoryMap.end())
	{
		showWarning(QString::fromLocal8Bit("Не найден требуемая фабрика объектов"));
		return;
	}

	const auto& creator = itFactory->second;
	auto dbObjState = creator->CreateObj();
	if (!dbObjState)
		return;

	QString error;
	if (!dbObjState->AddNewObjToModelAndThenToDB(model, selectedIdxs, error))
		showWarning(error);
}