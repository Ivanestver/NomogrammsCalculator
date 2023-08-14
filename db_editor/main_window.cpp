#include "main_window.h"
#include "db_executor.h"
#include <QToolBar>
#include <QUuid>
#include "TreeViewModel.h"
#include "db_state/properties.h"

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

	QString queryStr = "SELECT t1.template_id as template_id, class_id, property_value\
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
		item.classId = methodology_class;
		ids.push_back(item);
	}

	ui.treeView->setModel(new TreeViewModel(ids));
}

void MainWindow::addToolBar()
{
	QToolBar* toolBar = new QToolBar(this);
	toolBar->addAction(QString::fromLocal8Bit("�������� ����� ������"), this, &MainWindow::onAddItem);
}

void MainWindow::onAddItem()
{

}