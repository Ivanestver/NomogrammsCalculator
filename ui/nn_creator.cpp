#include "nn_creator.h"
#include <QAbstractItemModel>
#include <vector>
#include <deque>

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

		// ”наследовано через QAbstractItemModel
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
				return Q_INVOKABLE QString::fromLocal8Bit("—крытый слой є%1").arg(index.row());
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
		connect(model, &HiddenLayersTableModel::dataChanged, this, &DlgNNCreator::onDataChanged);
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

	void DlgNNCreator::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
	{
		updateMap();
	}

	void DlgNNCreator::updateMap()
	{
		auto* scene = dynamic_cast<NNGraphicsScene*>(ui.nnGraphicsView->scene());
		if (!scene)
			return;

		scene->ShowMap(info.inputParamsNumber, info.hiddenLayers);
	}
}