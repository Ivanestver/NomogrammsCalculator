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

using namespace nomogramms;

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

			return Q_INVOKABLE createIndex(row, column, (void*)&parentPtr->children[row]);
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
			return Q_INVOKABLE 1;
		}

		virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override
		{
			if (role != Qt::DisplayRole)
				return QVariant();

			const auto* indexObj = getItem(index);
			if (indexObj == rootItem)
				return Q_INVOKABLE QVariant();

			return Q_INVOKABLE indexObj->item->GetName();;
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
		TreeItem* rootItem = nullptr;
	};

	NomogrammsViewer::NomogrammsViewer(QWidget* parent/* = nullptr*/)
		: QDialog(parent)
		, ui(new Ui::NomogrammViewer())
	{
		ui->setupUi(this);

		setTree();
	}

	void NomogrammsViewer::setTree()
	{
		auto dbWrapper = db::DataBaseWrapper::GetDatabase();
		std::vector<QUuid> idsOfMethodologies;
		auto error = dbWrapper->GetTemplateIDsByClassID(nomogramms::Methodology::GetCID(), idsOfMethodologies);
		if (!error.isEmpty())
		{
			QMessageBox::critical(this, tr("Ошибка!"), error);
			return;
		}

		dbWrapper.reset();

		std::vector<SMethodology> methodologies;
		for (const auto& id : idsOfMethodologies)
			methodologies.push_back(std::make_shared<Methodology>(id));

		ui->nomogrammsTree->setModel(new NomogrammTreeModel(methodologies));
	}
}