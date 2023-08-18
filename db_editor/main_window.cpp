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

	std::vector<TreeItem> ids;
	for (const auto& result : results)
	{
		TreeItem item;
		item.id = result[0].toUuid();
		item.name = result[1].toString();
		item.classId = methodology_class;
		ids.push_back(item);
	}

	ui.treeView->setModel(new TreeItemModel(ids));
}

void MainWindow::addToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новый объект"), this, &MainWindow::onAddItem);
	toolBar->addAction(QString::fromLocal8Bit("Добавить новую методику"), this, &MainWindow::onAddMethodology);
	toolBar->addAction(QString::fromLocal8Bit("Удалить выбранный объект"), this, &MainWindow::onRemoveItem);
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

	bool ok;
	const auto text = QInputDialog::getText(this, QString::fromLocal8Bit("Введите название нового объекта"), QString::fromLocal8Bit("Добавление нового объекта"), QLineEdit::Normal, "", &ok);
	if (!ok || text.isEmpty())
		return;

	if (!model->insertRow(model->rowCount(QModelIndex()) + 1, QModelIndex()))
	{
		showWarning(QString::fromLocal8Bit("Ошибка при добавлении нового элемента"));
		return;
	}

	const auto added = model->index(model->rowCount(QModelIndex()) - 1, 1, QModelIndex());
	if (added.isValid())
	{
		model->setData(added, text, TreeItemModel::ModelRole::NameRole);
		model->setData(added, methodology_class, TreeItemModel::ModelRole::ClassIDRole);
	}

	QString error;
	if (!model->SaveIndexToDB(added, error))
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
	auto* model = dynamic_cast<TreeItemModel*>(ui.treeView->model());
	if (!model)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Ошибка при получении модели дерева"), QString::fromLocal8Bit("Ошибка"));
		return;
	}

	model->removeRow(selected.row(), selected.parent());
}

void MainWindow::onAddItem()
{
	auto* model = ui.treeView->model();
	if (!model)
	{
		showWarning(QString::fromLocal8Bit("Ошибка при получении модели дерева"));
		return;
	}

	const auto selectedIdx = ui.treeView->selectionModel()->selectedIndexes();
	if (selectedIdx.isEmpty())
	{
		showWarning(QString::fromLocal8Bit("Выберите родительский элемент!"));
		return;
	}

	const auto& selected = selectedIdx.first();
	const auto* item = static_cast<const TreeItem*>(selected.internalPointer());
	if (!item)
	{
		showWarning("Ошибка при получении выделенного элемента");
		return;
	}

	QUuid selectedItemClassId = item->classId;

	QString text;
	QUuid classId;

	if (selectedItemClassId == nomogramm_class)
	{
		DlgChooseItemType dlg(this);
		int response = dlg.exec();
		if (response == QDialog::Accepted)
		{
			text = dlg.GetName();
			classId = dlg.IsNomogramm() ? nomogramm_class : graphics_class;
		}
		else
		{
			qDebug() << "Отмена ввода";
			return;
		}
	}
	else
	{
		bool ok;
		text = QInputDialog::getText(this, QString::fromLocal8Bit("Введите название нового объекта"), QString::fromLocal8Bit("Добавление нового объекта"), QLineEdit::Normal, "", &ok);
		if (!ok || text.isEmpty())
		{
			showWarning(QString::fromLocal8Bit("Возникла ошибка при вводе названия объекта"));
			return;
		}

		if (selectedItemClassId == methodology_class)
			classId = nomogramm_class;
	}

	if (!model->insertRow(model->rowCount(selected), selected))
	{
		showWarning(QString::fromLocal8Bit("Ошибка при добавлении нового элемента"));
		return;
	}

	const auto added = model->index(model->rowCount(selected) - 1, 1, selected);
	if (added.isValid())
	{
		model->setData(added, text, TreeItemModel::ModelRole::NameRole);
		model->setData(added, classId, TreeItemModel::ModelRole::ClassIDRole);
	}

	const auto* treeModel = dynamic_cast<const TreeItemModel*>(model);
	if (!treeModel)
	{
		showWarning(QString::fromLocal8Bit("Модель не является моделью дерева"));
		return;
	}

	QString error;
	if (!treeModel->SaveIndexToDB(added, error))
		showWarning(error);
}