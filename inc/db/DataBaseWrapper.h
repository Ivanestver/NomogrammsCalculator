#pragma once
#include <QtSql/QSqlDatabase>
#include "common/macros.h"
#include <QUuid>
#include <QVariant>
#include <vector>
#include <memory>
#include <tuple>
#include "db_state/db_state.h"
#include <QtGui/QImage>
#include <vector>

namespace db
{
	DECL_UNIQUE(DataBaseWrapper);
	DECL_SHARED(DataBaseWrapper);

	using Image = std::pair<QByteArray, const char*>;

	struct NNModelInfo
	{
		NNModelInfo(const QString& name, const QString& netFile, const QUuid& id, int inp, int outp, const std::vector<int>& hidden)
			: NetName(name)
			, NetFile(netFile)
			, NetId(id)
			, inputFeatures(inp)
			, outputFeatures(outp)
			, hiddenFeatures(hidden)
		{}

		QString NetName;
		QString NetFile;
		QUuid NetId;
		int inputFeatures;
		int outputFeatures;
		std::vector<int> hiddenFeatures;
	};

	class DataBaseWrapper
	{
	public:
		static SDataBaseWrapper GetDatabase();
		~DataBaseWrapper() = default;

		_NODISCARD std::vector<QUuid> GetTemplateIDsByClassID(const QUuid& classID) const;
		_NODISCARD QString GetPropertyValueByIdAndTemplateID(const QUuid& attributeID, const QUuid& templateID, QString& error);
		_NODISCARD std::vector<QString> GetPropertiesByIDsAndObjID(const std::vector<QUuid>& attributes, const QUuid& templateID);

		bool AddNN(const QString& nnName, const QString& nnFileName, QString& error);
		_NODISCARD std::vector<NNModelInfo> GetNNModels();
		_NODISCARD NNModelInfo GetNNModelInfo(const QUuid& ModelID);

		bool ExecuteUpdate(const QString& query, const std::vector<QVariant>& params, QString& error);
		_NODISCARD std::vector<std::vector<QVariant>> ExecuteQuery(const QString& query, const std::vector<QVariant>& params, QString& error) const;

		_NODISCARD QImage LoadNomogrammPicture(const QUuid& NomogrammId, QString& error) const;

	private: 
		QString turnIDToStr(const QUuid& id) const;

		std::tuple<int, std::vector<int>, int> readNNStruct(const QString& netStruct) const;

		bool openConnection() const;
		void closeConnection() const;

	private:
		mutable QSqlDatabase db;
		db_state::SDBState dbState{ nullptr };
		DataBaseWrapper(const db_state::SDBState& state);
	};

}