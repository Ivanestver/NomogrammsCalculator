#include "ui/view_nn.h"
#include "db/DataBaseWrapper.h"

namespace ui
{
	DlgViewNN::DlgViewNN(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		fillNNList();
	}

	void DlgViewNN::fillNNList()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		auto nnsInfo = db->GetNNModels();
		QStringList nnsNames;
		for (const auto& pair : nnsInfo)
			nnsNames.append(pair.first);

		ui.NNNamesListWidget->addItems(nnsNames);
	}
}