#include "common/DBObject.h"
#include "db/DataBaseWrapper.h"
#include "db/properties.h"
#include "nomogramms/Graphics.h"
#include "nomogramms/Nomogramm.h"
#include "nomogramms/Methodology.h"
#include "common/MeasureUnit.h"

using namespace nomogramms;

DBObject::DBObject(const QUuid& id)
	: id(id)
{
	initialize();
}

DBObject::DBObject(const DBObject& other)
{
	this->id = other.id;
	this->name = other.name;
}

DBObject::~DBObject() = default;

QString DBObject::GetName() const
{
	return name;
}

QUuid DBObject::GetId() const
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

void DBObject::initialize()
{
	const auto* dbInstance = db::DataBaseWrapper::GetInstance();
	if (!dbInstance)
		return;

	name = dbInstance->GetAttributeByIdAndTemplateID(db::properties::dbobject_name, id);
	initFromDB();
}