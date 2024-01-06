#include "nomogramms_viewer.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <nomogramms/Nomogramm.h>
#include <nomogramms/Methodology.h>
#include <TreeItem.h>
#include <vector>
#include <map>
#include <QGraphicsPixmapItem>

using namespace nomogramms;

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
namespace ui
{
	class NomogrammTreeModel : public QAbstractItemModel
	{
	public:
		NomogrammTreeModel(const std::vector<SMethodology> v_, QObject* parent = nullptr)
			: QAbstractItemModel(parent)
			, rootItem(new TreeItem())
		{
			for (const auto& meth : v_)
			{
				TreeItem item;
				item.item = meth;
				item.parent = rootItem;
				rootItem->children.push_back(item);
			}

			initItem(*rootItem);
		}

		// Унаследовано через QAbstractItemModel
		virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override
		{
			const auto* parentPtr = getItem(parent);
			if (!parentPtr
				|| parentPtr->children.empty()
				|| parentPtr->children.size() <= (size_t)row)
				return Q_INVOKABLE QModelIndex();

			return Q_INVOKABLE createIndex(row, column, (void*)&parentPtr->children[(size_t)row]);
		}

		virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override
		{
			if (!child.isValid())
				return Q_INVOKABLE QModelIndex();

			const auto* childPtr = getItem(child);
			if (!childPtr || !childPtr->parent || childPtr->parent == rootItem)
				return Q_INVOKABLE QModelIndex();

			const auto it = std::find_if(childPtr->parent->children.begin(), childPtr->parent->children.end(), [&childPtr](const TreeItem& item)
				{
					return *childPtr == item;
				});

			if (it == childPtr->parent->children.end())
				return Q_INVOKABLE QModelIndex();

			return Q_INVOKABLE createIndex(it - childPtr->parent->children.begin(), 0, (void*)childPtr->parent);
		}

		virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override
		{
			const auto* parentPtr = getItem(parent);
			if (!parentPtr)
				return Q_INVOKABLE 0;


			return Q_INVOKABLE (int)parentPtr->children.size();
		}

		virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override
		{
			Q_UNUSED(parent);
			return Q_INVOKABLE 1;
		}

		virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override
		{
			if (role != Qt::DisplayRole)
				return QVariant();

			const auto* indexObj = getItem(index);
			if (indexObj == rootItem)
				return Q_INVOKABLE QVariant();

			return Q_INVOKABLE indexObj->item->GetName();
		}

		const SICalculeable GetICalculableFromIndex(const QModelIndex& index) const
		{
			const TreeItem* item = static_cast<const TreeItem*>(index.internalPointer());
			return std::dynamic_pointer_cast<ICalculeable>(item->item);
		}

		const QUuid GetItemClassIdFromIndex(const QModelIndex& index) const
		{
			const auto* item = static_cast<const TreeItem*>(index.internalPointer());
			return item->item->GetClassID();
		}

	private:
		void initItem(TreeItem& itemToInit)
		{
			for (auto& item : itemToInit.children)
			{
				item.parent = &itemToInit;
				std::vector<SDBObject> children;
				item.item->GetChildren(children);

				for (const auto& child : children)
				{
					TreeItem newTreeItem;
					newTreeItem.item = child;

					item.children.push_back(newTreeItem);
				}

				initItem(item);
			}
		}

		TreeItem* getItem(const QModelIndex& idx) const
		{
			if (!idx.isValid())
				return rootItem;

			auto* item = static_cast<TreeItem*>(idx.internalPointer());
			return item ? item : rootItem;
		}

	private:
		NomogrammTreeModel(const NomogrammTreeModel&) = delete;
		NomogrammTreeModel(NomogrammTreeModel&&) = delete;
		NomogrammTreeModel& operator=(const NomogrammTreeModel&) = delete;
		NomogrammTreeModel& operator=(NomogrammTreeModel&&) = delete;

	private:
		TreeItem* rootItem = nullptr;
	};

	NomogrammsViewer::NomogrammsViewer(QWidget* parent/* = nullptr*/)
		: QDialog(parent)
		, ui(new Ui::NomogrammViewer())
	{
		ui->setupUi(this);
		ui->graphicsView->setScene(new QGraphicsScene());

		setTree();
		setGraphicsTabViewMode(false);

		connect(ui->calcBtn, &QPushButton::clicked, this, &NomogrammsViewer::onCalcBtnClicked);
		connect(ui->resetBtn, &QPushButton::clicked, this, &NomogrammsViewer::onResetBtnClicked);
	}

	void NomogrammsViewer::setTree()
	{
		auto dbWrapper = db::DataBaseWrapper::GetDatabase();
		std::vector<QUuid> idsOfMethodologies;
		try
		{
			idsOfMethodologies = dbWrapper->GetTemplateIDsByClassID(nomogramms::Methodology::GetCID());
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка!"), QString::fromLocal8Bit(e.what()));
		}

		dbWrapper.reset();

		std::vector<SMethodology> methodologies;
		for (const auto& id : idsOfMethodologies)
			methodologies.push_back(std::make_shared<Methodology>(id));

		ui->nomogrammsTree->setModel(new NomogrammTreeModel(methodologies));

		connect(ui->nomogrammsTree->selectionModel(), &QItemSelectionModel::currentChanged, this, &NomogrammsViewer::onCurrentItemTreeChanged);
	}

	void NomogrammsViewer::setInputTable(const ValuesMap& params)
	{
		ui->inputTable->setRowCount(0);
		ui->outputTable->setRowCount(0);

		addParameterTypeToInputTable(ParameterType::Input, params);
		addParameterTypeToInputTable(ParameterType::Output, params);

		connect(ui->inputTable, &QTableWidget::itemChanged, this, &NomogrammsViewer::onItemChanged);
	}

	void NomogrammsViewer::addParameterTypeToInputTable(ParameterType type, const ValuesMap& parameters)
	{
		const auto inputIt = parameters.find(type);
		if (inputIt == parameters.end())
			return;

		auto* tableWidget = type == ParameterType::Input ? ui->inputTable : ui->outputTable;

		for (const auto& measureUnit : inputIt->second)
		{
			addMeasureUnitToInputTable(measureUnit, tableWidget);
		}
	}

	void NomogrammsViewer::addMeasureUnitToInputTable(const Value& measureUnit, QTableWidget* tableWidget)
	{
		if (!measureUnit.first)
			return;

		auto* item = new QTableWidgetItem(measureUnit.first->GetName());
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		item->setData(Qt::UserRole + 1, QVariant::fromValue(measureUnit.first));

		int insertedRowNumber = tableWidget->rowCount();
		tableWidget->insertRow(insertedRowNumber);
		tableWidget->setItem(insertedRowNumber, 0, item);

		item = new QTableWidgetItem(QString::number(measureUnit.second));
		tableWidget->setItem(insertedRowNumber, 1, item);
	}

	void NomogrammsViewer::setValue(const QTableWidgetItem* item)
	{
		const auto it = values.find(ParameterType::Input);
		assert(it != values.end());

		int rowNumber = ui->inputTable->row(item);
		if (rowNumber >= (int)it->second.size())
			return;

		double value = item->text().toDouble();
		it->second[(size_t)rowNumber].second = value;
	}

	nomogramms::IOData NomogrammsViewer::createInputData() const
	{
		IOData inputData;
		for (int i = 0; i < ui->inputTable->rowCount(); ++i)
		{
			const auto* item = ui->inputTable->item(i, 0);
			const auto measureUnit = item->data(Qt::UserRole + 1).value<SMeasureUnit>();

			item = ui->inputTable->item(i, 1);
			double value = item->text().toDouble();

			inputData.AddValue(measureUnit, value);
		}

		return inputData;
	}

	void NomogrammsViewer::setOutputData(const IOData& outputData)
	{
		auto it = values.find(ParameterType::Output);
		assert(it != values.end());

		for (auto& pair : it->second)
		{
			double value = outputData.GetValue(pair.first);
			pair.second = value;
		}
	}

	void NomogrammsViewer::setGraphicsTabViewMode(bool show)
	{
		ui->graphicsTab->setVisible(show);
	}

	void NomogrammsViewer::showNomogramm(const SNomogramm& Nomogramm)
	{
		setGraphicsTabViewMode(true);
		auto db = db::DataBaseWrapper::GetDatabase();
		if (!db)
			return;

		QString error;
		auto image = db->LoadNomogrammPicture(Nomogramm->GetId(), error);
		if (image.isNull())
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), error);
			return;
		}

		auto* item = new QGraphicsPixmapItem();
		item->setPixmap(QPixmap::fromImage(image));

		ui->graphicsView->scene()->clear();
		ui->graphicsView->scene()->addItem(item);
	}

	void NomogrammsViewer::onCurrentItemTreeChanged(const QModelIndex& current, const QModelIndex& previous)
	{
		Q_UNUSED(previous);
		ui->inputTable->setRowCount(0);
		ui->outputTable->setRowCount(0);

		const auto* model = dynamic_cast<const NomogrammTreeModel*>(ui->nomogrammsTree->model());
		if (!model)
			return;

		currentCalculeable = model->GetICalculableFromIndex(current);
		if (!currentCalculeable)
			return;

		const auto itemClassId = model->GetItemClassIdFromIndex(current);
		if (itemClassId.isNull())
			return;

		if (itemClassId == Nomogramm::GetCID())
		{
			showNomogramm(std::dynamic_pointer_cast<Nomogramm>(currentCalculeable));
		}
		else
			setGraphicsTabViewMode(false);

		ICalculeable::ParametersDict parameters;
		currentCalculeable->GetParameters(parameters);
		values.clear();

		for (const auto& pair : parameters)
		{
			std::vector<Value> valuesVector;
			for (const auto& measureUnit : pair.second)
				valuesVector.push_back({measureUnit, 0.0});
			values.insert({ pair.first, valuesVector });
		}

		disconnect(ui->inputTable, &QTableWidget::itemChanged, this, &NomogrammsViewer::onItemChanged);
		setInputTable(values);
	}

	void NomogrammsViewer::onItemChanged(QTableWidgetItem* item)
	{
		if (!currentCalculeable)
			return;

		setValue(item);
	}

	void NomogrammsViewer::onCalcBtnClicked()
	{
		const auto inputData = createInputData();
		IOData outputData;

		QString error;
		if (!currentCalculeable->Calculate(inputData, outputData, error))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), error);
			return;
		}

		disconnect(ui->inputTable, &QTableWidget::itemChanged, this, &NomogrammsViewer::onItemChanged);
		setOutputData(outputData);
		setInputTable(values);
	}

	void NomogrammsViewer::onResetBtnClicked()
	{
		for (auto& value : values)
		{
			for (auto& item : value.second)
			{
				item.second = 0.0;
			}
		}

		setInputTable(values);
	}

	void NomogrammsViewer::onSpinBoxValueChanged(int value)
	{
		Q_UNUSED(value);
	}
}