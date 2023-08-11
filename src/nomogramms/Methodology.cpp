#include "nomogramms/Methodology.h"
#include "db/DataBaseWrapper.h"
#include <QVariant>

namespace nomogramms
{
	Methodology::Methodology(const QUuid& id)
		: base(id)
	{
		initFromDB();
	}

	Methodology::Methodology(const Methodology& other)
		: base(other)
	{
		this->nomogramms.insert(this->nomogramms.begin(), other.nomogramms.begin(), other.nomogramms.end());
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
		QString queryString = "select [sub_id] from [template_template] where master_id = :1";
		std::vector<QVariant> params{ QVariant(GetId()) };
		QString error;
		auto result = db->ExecuteQuery(queryString, params, error);
		if (!error.isEmpty())
			return;

		for (const auto& v : result)
		{
			for (const auto& itemID : v)
			{
				if (itemID.type() == QVariant::Uuid)
				{
					auto nomogramm = std::make_shared<Nomogramm>(itemID.value<QUuid>());
					nomogramms.push_back(nomogramm);
				}
			}
		}
	}

	bool Methodology::operator==(const DBObject& other)
	{
		const auto& o = static_cast<const Methodology&>(other);
		return base::operator==(other)
			&& this->nomogramms == o.nomogramms;
	}

	bool Methodology::operator!=(const DBObject& other)
	{
		return !operator==(other);
	}

	Methodology& Methodology::operator=(const DBObject& other)
	{
		if (*this == other)
			return *this;

		base::operator=(other);
		const auto& o = static_cast<const Methodology&>(other);
		this->nomogramms = o.nomogramms;

		return *this;
	}

	const std::vector<SNomogramm>& Methodology::GetNomogramms() const
	{
		return nomogramms;
	}
}