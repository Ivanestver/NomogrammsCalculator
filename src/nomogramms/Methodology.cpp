#include "nomogramms/Methodology.h"
#include "db/DataBaseWrapper.h"
#include <QVariant>
#include <QDebug>

namespace nomogramms
{
	Methodology::Methodology(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	bool Methodology::GetChildren(std::vector<SDBObject>& children) const
	{
		if (nomogramms.empty())
			return false;

		children.clear();
		children.insert(children.begin(), nomogramms.begin(), nomogramms.end());
		
		return true;
	}

	void Methodology::initFromDB()
	{
		auto db = db::DataBaseWrapper::GetDatabase();
		QString queryString = "select sub_id from template_template where master_id = ?";
		std::vector<QVariant> params{ QVariant(GetId()) };
		QString error;
		auto result = db->ExecuteQuery(queryString, params, error);
		if (!error.isEmpty())
			return;

		for (const auto& v : result)
		{
			for (const auto& itemID : v)
			{
				auto nomogramm = std::make_shared<Nomogramm>(itemID.value<QUuid>());
				nomogramms.push_back(nomogramm);
			}
		}
	}

	bool Methodology::operator==(const DBObject& other) const
	{
		const auto& o = static_cast<const Methodology&>(other);
		return base::operator==(other)
			&& this->nomogramms == o.nomogramms;
	}

	bool Methodology::operator!=(const DBObject& other) const
	{
		return !operator==(other);
	}

	const std::vector<SNomogramm>& Methodology::GetNomogramms() const
	{
		return nomogramms;
	}
}