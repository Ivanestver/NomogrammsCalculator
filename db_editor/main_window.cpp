#include "main_window.h"
#include "db_executor.h"
#include <QToolBar>
#include <QUuid>
#include "TreeViewModel.h"
#include "db_state/properties.h"
#include <QInputDialog>
#include "choose_item_type_dlg.h"
#include <QDebug>

const QUuid methodology_class("A8A4951D-8542-4CFA-B593-ECBA3DE727D1");
const QUuid nomogramm_class("F5313633-C8FC-43DC-A92E-88B7EE8DF439");
const QUuid rule_class("BB75693A-4506-47DE-9DE6-6F40C8BC3C74");
const QUuid graphics_class("6DCE13AE-8897-48EC-9B4E-664845D40D73");

MainWindow::MainWindow(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initTree();
	addToolBar();
	addContextMenuToTree();

	factoryMap.insert({ methodology_class, std::make_shared<StateCreator<MethodologyState>>()});
	factoryMap.insert({ nomogramm_class, std::make_shared<NomogrammGraphicStateCreator>()});
	factoryMap.insert({ graphics_class, std::make_shared<NomogrammGraphicStateCreator>()});

	factoryMapIndependent.insert({ methodology_class, std::make_shared<StateCreator<MethodologyState>>()});
	factoryMapIndependent.insert({ nomogramm_class, std::make_shared<StateCreator<NomogrammState>>()});
	factoryMapIndependent.insert({ graphics_class, std::make_shared<StateCreator<GraphicState>>()});

	mappingRules.insert({QUuid(), methodology_class});
	mappingRules.insert({methodology_class, nomogramm_class});
	mappingRules.insert({ nomogramm_class, nomogramm_class });
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
	dbExecutor->ExecSELECT(queryStr, { DBExecutor::DBExecutorUtils::TurnUuidToStr(methodology_class), DBExecutor::DBExecutorUtils::TurnUuidToStr(db_state::properties::dbobject_name)}, results);

	std::vector<STreeItem> ids;
	for (const auto& result : results)
	{
		auto item = std::make_shared<TreeItem>();
		item->id = result[0].toUuid();
		item->name = result[1].toString();
		item->classId = methodology_class;
		ids.push_back(item);
	}

	ui.treeView->setModel(new TreeItemModel(ids));
	ui.treeView->selectAll();
}

void MainWindow::addToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новый объект"), this, &MainWindow::onAddItem);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новую методику"), this, &MainWindow::onAddMethodology);
	toolBar->addAction(QString::fromLocal8Bit("Удалить выбранный объект"), this, &MainWindow::onRemoveItem);
}

void MainWindow::addContextMenuToTree()
{
	ui.treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(ui.treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onCustomMenuRequested);

	treeContextMenu = new QMenu(ui.treeView);
	treeContextMenu->addAction(QString::fromLocal8Bit("Удалить выбранный объект"), this, &MainWindow::onRemoveItem);
}

QMessageBox::StandardButton MainWindow::showWarning(const QString& message)
{
	return QMessageBox::warning(this, QString::fromLocal8Bit("Ошибка"), message);
}

void MainWindow::onAddMethodology()
{
	auto* model = dynamic_cast<TreeItemModel*>(ui.treeView->model());
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
	const auto selectedIdxs = ui.treeView->selectionModel()->selectedIndexes();
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
	if (!state->RemoveItem(selected, ui.treeView->model(), error))
		showWarning(error);
}

void MainWindow::onCustomMenuRequested(const QPoint& point)
{
	QModelIndex index = ui.treeView->indexAt(point);
	if (index.isValid()) {
		treeContextMenu->exec(ui.treeView->viewport()->mapToGlobal(point));
	}
}

void MainWindow::onAddItem()
{
	auto* model = ui.treeView->model();
	if (!model)
	{
		showWarning(QString::fromLocal8Bit("Ошибка при получении модели дерева"));
		return;
	}

	const auto selectedIdxs = ui.treeView->selectionModel()->selectedIndexes();
	if (selectedIdxs.isEmpty())
	{
		showWarning(QString::fromLocal8Bit("Выберите родительский элемент!"));
		return;
	}

	const auto& selected = selectedIdxs.first();
	const auto* selectedItem = static_cast<const TreeItem*>(selected.internalPointer());
	auto it = mappingRules.find(selectedItem->classId);
	if (it == mappingRules.end())
	{
		showWarning(QString("Нет правила отражения для класса с ID = %1").arg(selectedItem->classId.toString()));
		return;
	}

	const auto& creator = factoryMap.find(it->second)->second;
	auto dbObjState = creator->CreateObj();
	QString error;
	if (!dbObjState->AddNewObjToModelAndThenToDB(model, selectedIdxs, error))
		showWarning(error);
}