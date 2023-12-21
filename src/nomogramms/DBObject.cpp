#include "nomogramms/DBObject.h"
#include "db/DataBaseWrapper.h"
#include "db_state/properties.h"
#include "nomogramms/Graphics.h"
#include "nomogramms/Nomogramm.h"
#include "nomogramms/Methodology.h"
#include "nomogramms/MeasureUnit.h"
#include "exceptions/db_exception.h"
#include <QDebug>

using namespace nomogramms;

DBObject::DBObject(const QUuid& id)
	: id(id)
{
	initFromDB();
}

const QString& DBObject::GetName() const
{
	return name;
}

const QUuid& DBObject::GetId() const
{
	return id;
}

bool DBObject::operator==(const DBObject& other) const
{
	return this->id == other.id
		&& this->name == other.name;
}

bool DBObject::operator!=(const DBObject& other) const
{
	return !operator==(other);
}

bool DBObject::GetChildren(std::vector<SDBObject>& children) const
{
	Q_UNUSED(children);
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
		QString error;
		name = dbInstance->GetPropertyValueByIdAndTemplateID(db_state::properties::dbobject_name, id, error);
	}
	catch (const exceptions::BadRequestException& e)
	{
		qDebug() << e.GetMessage();
		return;
	}
}

void DBObject::setName(const QString& name_)
{
	this->name = name_;
}