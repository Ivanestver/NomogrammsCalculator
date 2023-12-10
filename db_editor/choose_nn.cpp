#include "choose_nn.h"
#include "db_executor.h"
#include <QMessageBox>

DlgChooseNN::DlgChooseNN(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	connect(m_ui.nnListWidget, &QListWidget::currentRowChanged, this, &DlgChooseNN::onListItemClicked);
	loadNets();
}

const std::pair<QString, QUuid>& DlgChooseNN::GetChosenItemInfo() const
{
	if (chosenRow == -1)
		return {};

	return m_items[chosenRow];
}

void DlgChooseNN::onListItemClicked(int row)
{
	if (row < 0 || row >= m_items.size())
		return;
	
	chosenRow = row;
}

void DlgChooseNN::loadNets()
{
	const auto dbExecutor = DBExecutor::GetInstance();
	if (!dbExecutor)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Отсутствует подключение к БД"));
		return;
	}

	DBExecutor::Response response;
	QString error;
	if (!dbExecutor->ExecSELECT("select net_name, net_id from nets", {}, response, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Не удалось получить список нейронных сетей"));
		return;
	}

	for (const auto& record : response)
	{
		if (record.empty())
			continue;

		auto nnName = record[0].toString();
		m_ui.nnListWidget->addItem(nnName);

		m_items.emplace_back(nnName, record[1].toUuid());
	}
}
