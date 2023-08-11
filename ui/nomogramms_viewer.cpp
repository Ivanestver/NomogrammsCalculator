#include "nomogramms_viewer.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <nomogramms/Nomogramm.h>
#include <nomogramms/Methodology.h>

using namespace nomogramms;

namespace ui
{
	class NomogrammTreeModel : public QAbstractItemModel
	{
	public:
		NomogrammTreeModel(const std::vector<SMethodology> v_, QObject* parent = nullptr)
			: QAbstractItemModel(parent)
			, v(v_)
		{}

		// Унаследовано через QAbstractItemModel
		virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent) const override
		{
			if (!parent.isValid())
				return Q_INVOKABLE createIndex(row, column, v[row].get());

			const DBObject* parentObj = static_cast<const DBObject*>(parent.internalPointer());
			std::vector<SDBObject> children;
			if (!parentObj->GetChildren(children)
			  || (int)children.size() <= row)
				return QModelIndex();

			return Q_INVOKABLE createIndex(row, column, children[row].get());
		}

		virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override
		{
			return Q_INVOKABLE QModelIndex();
		}

		virtual Q_INVOKABLE int rowCount(const QModelIndex& parent) const override
		{
			if (!parent.isValid())
				return int(v.size());

			const DBObject* parentObj = static_cast<const DBObject*>(parent.internalPointer());
			std::vector<SDBObject> children;
			if (!parentObj->GetChildren(children))
				return 0;

			return Q_INVOKABLE (int)children.size();
		}

		virtual Q_INVOKABLE int columnCount(const QModelIndex& parent) const override
		{
			return Q_INVOKABLE 1;
		}

		virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role) const override
		{
			if (role != Qt::DisplayRole)
				return QVariant();

			const DBObject* indexObj = static_cast<const DBObject*>(index.internalPointer());
			return Q_INVOKABLE indexObj->GetName();
		}

	private:
		std::vector<SMethodology> v;
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