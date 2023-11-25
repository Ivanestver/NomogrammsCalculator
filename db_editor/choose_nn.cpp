#include "choose_nn.h"
#include "db_executor.h"
#include <QMessageBox>

DlgChooseNN::DlgChooseNN(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	connect(m_ui.nnListWidget, &QListWidget::currentTextChanged, this, &DlgChooseNN::onListItemClicked);
	loadNets();
}

const QString& DlgChooseNN::GetChosenNNName() const
{
	return m_chosenNNName;
}

void DlgChooseNN::onListItemClicked(const QString& currentText)
{
	m_chosenNNName = currentText;
}

void DlgChooseNN::loadNets()
{
	const auto dbExecutor = DBExecutor::GetInstance();
	if (!dbExecutor)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("����������� ����������� � ��"));
		return;
	}

	DBExecutor::Response response;
	QString error;
	if (!dbExecutor->ExecSELECT("select net_name from nets", {}, response, error))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("�� ������� �������� ������ ��������� �����"));
		return;
	}

	for (const auto& record : response)
	{
		if (record.empty())
			continue;

		auto nnName = record.front().toString();
		m_ui.nnListWidget->addItem(nnName);
	}
}
