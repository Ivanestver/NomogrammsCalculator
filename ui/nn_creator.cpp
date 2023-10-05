#include "nn_creator.h"
#include <QAbstractItemModel>
#include <vector>
#include <deque>
#include <set>
#include <QFile>
#include <QFileDialog>
#include <QTableWidget>
#include "ui/learning_stats.h"
#include "ui/test_nn.h"
#include "xml/xml.h"
#include <QMessageBox>
#include "db/DataBaseWrapper.h"
#include <QInputDialog>

namespace ui
{
	class NeuroLayerGraphicsItem
	{
	public:
		NeuroLayerGraphicsItem(int neuronsNumber_, double offsetY_, double radius_)
			: neuronsNumber(neuronsNumber_)
			, offsetY(offsetY_)
			, radius(radius_)
		{
			radiusPlusHalfSpace = radius + space / 2;
			diameter = radius * 2;
		}

		~NeuroLayerGraphicsItem() = default;

		void DrawLayer(QGraphicsScene* scene)
		{
			if (neuronsNumber == 0)
				return;

			for (int i = neuronsNumber; i > 0; i -= 2)
			{
				const double x = getDistanceFromCenter(i);
				const double y = offsetY - radius;
				scene->addEllipse(-x, y, diameter, diameter, QPen(Qt::black), QBrush(Qt::black));
				scene->addEllipse(x - diameter, y, diameter, diameter, QPen(Qt::black), QBrush(Qt::black));
				points.push_front(QPointF(-x, y));
				points.push_back(QPointF(x - diameter, y));
			}
		}

		std::vector<QPointF> GetPoints() const
		{
			std::vector<QPointF> points_;
			for (const auto& point : points)
				points_.push_back(point);

			return points_;
		}

	private:
		double getDistanceFromCenter(int neuronsCount) const
		{
			return (neuronsCount - 1) * radiusPlusHalfSpace + radius;
		}

	private:
		int neuronsNumber{ 0 };
		double offsetY{ 0.0 };
		double radius{ 0.0 };
		double diameter{ 0.0 };
		double border = 5.;
		double space = 10.;
		double radiusPlusHalfSpace{ 0.0 };
		std::deque<QPointF> points;
	};

	class NNGraphicsScene : public QGraphicsScene
	{
	public:
		NNGraphicsScene(QObject* parent = nullptr)
			: QGraphicsScene(parent)
		{}
		~NNGraphicsScene() override = default; 

		void ShowMap(int inputParamsNumber, const std::vector<int>& hiddenLayers)
		{
			clear();
			std::vector<NeuroLayerGraphicsItem> layers;

			//drawCoordinateLines();
			double offset = 0.;
			NeuroLayerGraphicsItem inputItem(inputParamsNumber, offset, radius);
			inputItem.DrawLayer(this);
			layers.push_back(inputItem);

			double diameter = radius * 2;

			for (size_t i = 0; i < hiddenLayers.size(); i++)
			{
				offset += 3 * diameter;
				NeuroLayerGraphicsItem item(hiddenLayers[i], offset, radius);
				item.DrawLayer(this);
				layers.push_back(item);
			}

			NeuroLayerGraphicsItem outputItem(1, offset + 3 * diameter, radius);
			outputItem.DrawLayer(this);
			layers.push_back(outputItem);

			printConnections(layers);
		}

	private:
		void drawCoordinateLines()
		{
			addLine(-100, 0, 100, 0, QPen(Qt::black));
			addLine(0, -100, 0, 100, QPen(Qt::black));
		}

		void printConnections(const std::vector<NeuroLayerGraphicsItem>& layers)
		{
			for (size_t i = 1; i < layers.size(); i++)
			{
				auto pointsOfPrevLayer = layers[i - 1].GetPoints();
				auto pointsOfCurrentLayer = layers[i].GetPoints();

				for (const auto& prevPoint : pointsOfPrevLayer)
				{
					for (const auto& currPoint : pointsOfCurrentLayer)
					{
						addLine(prevPoint.x() + radius, prevPoint.y() + radius, currPoint.x() + radius, currPoint.y() + radius, QPen(Qt::black));
					}
				}
			}
		}

	private:
		double radius = 10.;
	};

	class HiddenLayersTableModel : public QAbstractItemModel
	{
	public:
		HiddenLayersTableModel(DlgNNCreatorInfo& info_, QObject* parent = nullptr)
			: QAbstractItemModel(parent)
			, info(info_)
		{}

		~HiddenLayersTableModel() override = default;

		// Унаследовано через QAbstractItemModel
		virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override
		{
			Q_UNUSED(parent);

			if (row >= (int)info.hiddenLayers.size())
				return Q_INVOKABLE QModelIndex();

			if (column == 0)
				return Q_INVOKABLE createIndex(row, column, (quintptr)nullptr);

			return Q_INVOKABLE createIndex(row, column, (quintptr)info.hiddenLayers[row]);
		}

		virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override
		{
			return Q_INVOKABLE QModelIndex();
		}

		virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override
		{
			Q_UNUSED(parent);
			return Q_INVOKABLE (int)info.hiddenLayers.size();
		}

		virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override
		{
			Q_UNUSED(parent);
			return Q_INVOKABLE 2;
		}

		virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override
		{
			if (!index.isValid() || role != Qt::DisplayRole)
				return Q_INVOKABLE QVariant();

			if (index.column() == 0)
				return Q_INVOKABLE QString::fromLocal8Bit("Скрытый слой №%1").arg(index.row());
			else
				return Q_INVOKABLE QString::number(info.hiddenLayers[index.row()]);
		}

		virtual Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override
		{
			if (role != Qt::EditRole)
				return false;

			if (index.column() != 1)
				return false;

			if (index.row() >= (int)info.hiddenLayers.size())
				return false;

			if (!value.isValid() && value.type() != QVariant::Type::Int)
				return false;

			info.hiddenLayers[index.row()] = value.toInt();
			Q_EMIT QAbstractItemModel::dataChanged(index, index);

			return true;
		}

		void SetLayersNumber(int layersNumber)
		{
			int difference = layersNumber - (int)info.hiddenLayers.size();
			if (difference > 0)
			{
				beginInsertRows(QModelIndex(), (int)info.hiddenLayers.size(), (int)info.hiddenLayers.size() + difference - 1);
				for (int i = 0; i < difference; ++i)
					info.hiddenLayers.push_back(1);

				endInsertRows();
			}
			else
			{
				beginRemoveRows(QModelIndex(), (int)info.hiddenLayers.size() + difference, (int)info.hiddenLayers.size() - 1);
				for (int i = difference; i < 0; ++i)
					info.hiddenLayers.pop_back();
				endRemoveRows();
			}
		}

		virtual Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex& index) const
		{
			if (!index.isValid() || index.column() == 0)
				return QAbstractItemModel::flags(index);

			return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
		}

	private:
		DlgNNCreatorInfo& info;
	};

	DlgNNCreator::DlgNNCreator(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);

		ui.nnGraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

		auto* scene = new NNGraphicsScene();
		ui.nnGraphicsView->setScene(scene);

		auto* model = new HiddenLayersTableModel(info);
		ui.hiddenLayersTableView->setModel(model);

		connect(ui.inputParamsSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &DlgNNCreator::onInputParamsNumberChanged);
		connect(ui.hiddenLayersSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &DlgNNCreator::onLayersCountChanged);
		connect(model, &HiddenLayersTableModel::dataChanged, this, &DlgNNCreator::onHiddenNeuronsCountChanged);
		connect(ui.viewDataBtn, &QPushButton::clicked, this, &DlgNNCreator::onViewDataBtnClicked);

		connect(ui.chooseFileBtn, &QPushButton::clicked, this, &DlgNNCreator::onChooseFileBtnClicked);
		connect(ui.startLearningBtn, &QPushButton::clicked, this, &DlgNNCreator::onStartLearningBtnClicked);
		connect(ui.showStatBtn, &QPushButton::clicked, this, &DlgNNCreator::onShowLearningStatsBtnClicked);

		connect(ui.testBtn, &QPushButton::clicked, this, &DlgNNCreator::onTestNNBtnClicked);
		//connect(ui.splitCheckBox, &QCheckBox::clicked, this, &DlgNNCreator::onSpliCheckboxClicked);
		connect(ui.splitCheckBox, &QCheckBox::clicked, ui.trainPercentSpinBox, &QSpinBox::setEnabled);
		connect(ui.saveNNBtn, &QPushButton::clicked, this, &DlgNNCreator::onSaveNNBtnClicked);

		auto* lossScene = new QGraphicsScene();
		ui.lossGraphicsView->setScene(lossScene);

		updateMap();
	}

	void DlgNNCreator::onInputParamsNumberChanged(int value)
	{
		info.inputParamsNumber = value;
		updateMap();
	}

	void DlgNNCreator::onLayersCountChanged(int value)
	{
		auto* model = dynamic_cast<HiddenLayersTableModel*>(ui.hiddenLayersTableView->model());
		if (!model)
			return;

		model->SetLayersNumber(value);
		updateMap();
	}

	void DlgNNCreator::onHiddenNeuronsCountChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
	{
		updateMap();
	}

	void DlgNNCreator::onChooseFileBtnClicked()
	{
		inputData.clear();
		QString defaultFilter("CSV Files (*.csv);");
		QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Выберите файл"), "./", "CSV files (*.csv)", &defaultFilter, QFileDialog::Option::DontUseNativeDialog);

		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly))
			return;

		while (!file.atEnd())
		{
			auto line = file.readLine();
			auto items = line.split(',');
			std::vector<QString> row;
			for (const auto& item : items)
			{
				row.push_back(QString(item));
			}
			inputData.push_back(row);
		}
	}

	void DlgNNCreator::onViewDataBtnClicked()
	{
		QDialog dlg(this);
		
		setupUIForViewDataDlg(&dlg);
		const auto& rect = geometry();
		dlg.setGeometry(rect.width() / 2, rect.height() / 2, rect.width() / 2, rect.height() / 2);
		dlg.exec();
	}

	void DlgNNCreator::onStartLearningBtnClicked()
	{
		ui.logListWidget->clear();
		ml::NNCouchSettings settings;
		settings.epochsCount = ui.epochsCountSpinBox->value();
		settings.batchSize = 4;
		settings.learningRate = ui.lrSpinBox->value();
		settings.nn = createNN();
		settings.criterion = createCriterion();
		settings.optimizerType = createOptimizer();
		
		auto* thisPtr = this;
		std::thread t([thisPtr, settings]()
		{
			std::shared_ptr<ml::NNCouch> couch = std::make_shared<ml::NNCouch>(settings);
			auto onEpochFinishedLambda = [thisPtr](const ml::LearningReply& reply)
			{
				thisPtr->onEpochFinished(reply);
			};

			auto onDecadeFinishedLambda = [thisPtr](const ml::LearningReply& reply)
			{
				thisPtr->onDecadeFinished(reply);
			};

			auto onErrorRaisedLambda = [thisPtr](const QString& error)
			{
				thisPtr->onErrorRaised(error);
			};
			connect(couch.get(), &ml::NNCouch::EpochFinished, onEpochFinishedLambda);
			connect(couch.get(), &ml::NNCouch::DecadeFinished, onDecadeFinishedLambda);
			connect(couch.get(), &ml::NNCouch::ErrorRaised, onErrorRaisedLambda);

			auto [xTrain, yTrue, xVal, yVal] = thisPtr->splitData();
			couch->Train(xTrain, yTrue, { xVal, yVal });

			thisPtr->learningStatistics = couch->GetStatistics();

		});

		t.detach();
	}

	void DlgNNCreator::onShowLearningStatsBtnClicked()
	{
		LearningStatsDlg dlg(learningStatistics, this);
		dlg.exec();
	}

	void DlgNNCreator::onTestNNBtnClicked()
	{
		TestNNInfo testInfo;
		testInfo.nnName = ui.nnNameLineEdit->text();
		testInfo.inputParamsNumber = info.inputParamsNumber;
		testInfo.nn = nn;
		testInfo.outputParamsNumber = 1;

		TestNNDlg dlg(testInfo, this);
		dlg.exec();
	}

	void DlgNNCreator::onSaveNNBtnClicked()
	{
		const auto* xmlConfiguration = xml::XmlConfiguration::GetInstance();
		if (!xmlConfiguration)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание!"), QString::fromLocal8Bit("Ошибка возникла при попытке прочитать кофигурационный файл"));
			return;
		}

		auto fileName = QInputDialog::getText(this, QString::fromLocal8Bit("Название файла для сохранения"), QString::fromLocal8Bit("Введите название файла, где сохранить модель"));
		if (fileName.isEmpty())
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Введите название файла"));
			return;
		}

		QString modelsPath = xmlConfiguration->GetValueByTag("models_path");
		torch::serialize::OutputArchive output;
		nn->save(output);
		modelsPath += fileName;
		modelsPath += ".pt";
		try
		{
			output.save_to(modelsPath.toStdString());
			auto db = db::DataBaseWrapper::GetDatabase();
			QString error;
			if (!db->AddNN(QString::fromStdString(nn->name()), fileName, error))
			{
				QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"), error);
				return;
			}
		}
		catch (std::exception& e)
		{
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка!"), e.what());
		}
	}

	void DlgNNCreator::onEpochFinished(const ml::LearningReply& reply)
	{
		ui.logListWidget->addItems({ QString::fromLocal8Bit("Эпоха №%1").arg(reply.epochNumber),
			QString::fromLocal8Bit("Лосс по эпохе: %1").arg(reply.avgLoss) });

		losses.push_back(reply.avgLoss);
		auto* scene = ui.lossGraphicsView->scene();
		if (!scene || losses.size() == 1uLL)
			return;
		
		size_t s = losses.size() - 1;
		scene->addLine(s - 1, losses[s - 1] * -100, s, losses[s] * -100);
	}

	void DlgNNCreator::onDecadeFinished(const ml::LearningReply& reply)
	{
		ui.logListWidget->addItem(reply.message);
		ui.logListWidget->addItem(QString::fromLocal8Bit("Лосс на валидации: %1").arg(reply.valLoss));
	}

	void DlgNNCreator::onErrorRaised(const QString& error)
	{
		ui.logListWidget->addItem(error);
	}

	void DlgNNCreator::onSpliCheckboxClicked(bool checked)
	{
		ui.trainPercentSpinBox->setEnabled(checked);
	}

	void DlgNNCreator::updateMap()
	{
		auto* scene = dynamic_cast<NNGraphicsScene*>(ui.nnGraphicsView->scene());
		if (!scene)
			return;

		scene->ShowMap(info.inputParamsNumber, info.hiddenLayers);
	}

	void DlgNNCreator::setupUIForViewDataDlg(QDialog* viewDataDlg)
	{
		auto* dlgLayout = new QVBoxLayout(viewDataDlg);

		auto* tableWidget = new QTableWidget();
		dlgLayout->addWidget(tableWidget);

		if (inputData.empty())
			return;

		tableWidget->setColumnCount(inputData[0].size());
		for (size_t i = 0; i < inputData.size(); ++i)
		{
			tableWidget->insertRow(i);
			for (size_t j = 0; j < inputData[i].size(); ++j)
			{
				tableWidget->setItem(i, j, new QTableWidgetItem(inputData[i][j]));
			}
		}		
	}

	std::shared_ptr<ml::FullyConnectedNN> DlgNNCreator::createNN()
	{
		nn.reset();
		nn = std::make_shared<ml::FullyConnectedNN>(info.inputParamsNumber, info.hiddenLayers, ui.OutputParamsSpinBox->value(), ui.nnNameLineEdit->text());
		return nn;
	}

	std::shared_ptr<ml::Criterion> DlgNNCreator::createCriterion() const
	{
		return std::make_shared<ml::MSECriterion>(torch::nn::MSELoss());
	}

	ml::OptimizerType DlgNNCreator::createOptimizer() const
	{
		return ml::OptimizerType::Adam;
	}

	DlgNNCreator::TrainValData DlgNNCreator::splitData() const
	{
		if (inputData.empty())
			return TrainValData();

		at::Tensor xTrain = torch::zeros({(long long)inputData.size(), (long long)inputData[0].size() - 1});
		auto yTrue = torch::zeros({ (long long)inputData.size(), 1 });

		for (size_t row = 0; row < inputData.size(); ++row)
		{
			size_t rowSize = inputData[row].size() - 1;
			for (size_t col = 0; col < rowSize; ++col)
			{
				xTrain[row][col] = inputData[row][col].toDouble();
			}

			yTrue[row][0] = inputData[row][rowSize].toDouble();
		}

		if (ui.splitCheckBox->isChecked())
		{
			double percent = ui.trainPercentSpinBox->value();
			int size = xTrain.size(0);
			int trainSize = static_cast<int>(percent * size / 100);
			int valSize = size - trainSize;
			
			std::set<int> rowNumbersForVal;
			while (rowNumbersForVal.size() < valSize)
			{
				rowNumbersForVal.insert(torch::randint(size, 1).item().toInt());
			}

			at::Tensor valX = torch::zeros({ valSize, 1 });
			at::Tensor valY = torch::zeros({ valSize, 1 });

			int i = 0;
			for (int rowNumber : rowNumbersForVal)
			{
				valX[i][0] = xTrain[rowNumber][0];
				valY[i][0] = yTrue[rowNumber][0];
				++i;
			}

			at::Tensor trainX = torch::zeros({ trainSize, 1 });
			at::Tensor trainY = torch::zeros({ trainSize, 1 });

			for (int i = 0, j = 0; i < size; ++i)
			{
				const auto found = rowNumbersForVal.find(i);
				if (found != rowNumbersForVal.end())
				{
					rowNumbersForVal.erase(found);
					continue;
				}

				trainX[j][0] = xTrain[i][0];
				trainY[j][0] = yTrue[i][0];
				++j;
			}

			return TrainValData(xTrain, yTrue, valX, valY);
		}
		else
			return TrainValData(xTrain, yTrue, {}, {});
	}

	void DlgNNCreator::drawLosses()
	{
		auto* scene = ui.lossGraphicsView->scene();
		scene->clear();
		for (size_t i = 1; i < losses.size(); ++i)
		{
			scene->addLine(i - 1, losses[i - 1] * -100, i, losses[i] * -100);
		}
	}
}