#include "common/DBObject.h"
#include "db/DataBaseWrapper.h"
#include "db_state/properties.h"
#include "nomogramms/Graphics.h"
#include "nomogramms/Nomogramm.h"
#include "nomogramms/Methodology.h"
#include "common/MeasureUnit.h"
#include "exceptions/db_exception.h"
#include <QDebug>

using namespace nomogramms;

DBObject::DBObject(const QUuid& id)
	: id(id)
{
	initFromDB();
}

DBObject::DBObject(const DBObject& other)
{
	this->id = other.id;
	this->name = other.name;
}

DBObject::~DBObject() = default;

const QString& DBObject::GetName() const
{
	return name;
}

const QUuid& DBObject::GetId() const
{
	return id;
}

bool DBObject::operator==(const DBObject& other)
{
	return this->id == other.id
		&& this->name == other.name;
}

bool DBObject::operator!=(const DBObject& other)
{
	return !operator==(other);
}

DBObject& DBObject::operator=(const DBObject& other)
{
	if (*this == other)
		return *this;

	this->id = other.id;
	this->name = other.name;

	return *this;
}

bool DBObject::GetChildren(std::vector<SDBObject>& children) const
{
	return false;
}

SDBObject DBObject::CreateDBObject(const QUuid& class_id, const QUuid& template_id)
{
	if (class_id == nomogramms::Graphics::GetCID())
		return std::make_shared<Graphics>(template_id);

	if (class_id == nomogramms::Measure::GetCID())
		return std::make_shared<Measure>(template_id);

	if (class_id == nomogramms::Nomogramm::GetCID())
		return std::make_shared<Nomogramm>(template_id);

	if (class_id == nomogramms::Methodology::GetCID())
		return std::make_shared<Methodology>(template_id);

	if (class_id == nomogramms::MeasureUnit::GetCID())
		return std::make_shared<MeasureUnit>(template_id);

	return nullptr;
}

void DBObject::initFromDB()
{
	auto dbInstance = db::DataBaseWrapper::GetDatabase();
	if (!dbInstance)
		return;
	try
	{
		name = dbInstance->GetPropertyValueByIdAndTemplateID(db::properties::dbobject_name, id);
	}
	catch (const exceptions::BadRequestException& e)
	{
		qDebug() << e.GetMessage();
		return;
	}
}