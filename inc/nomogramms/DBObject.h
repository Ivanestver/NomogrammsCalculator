#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include "common/macros.h"
#include <memory>
#include "db/DataBaseWrapper.h"

DECL_SHARED(DBObject);

class DBObject : public QObject
{
protected:
	using base = DBObject;
public:
	explicit DBObject(const QUuid& id);
	virtual ~DBObject() override = default;
	DBObject(const DBObject&) = default;
	DBObject(DBObject&&) noexcept = default;
	DBObject& operator=(const DBObject&) = default;
	DBObject& operator=(DBObject&&) noexcept = default;

	const QString& GetName() const;
	const QUuid& GetId() const;

	virtual bool operator==(const DBObject& other) const;
	virtual bool operator!=(const DBObject& other) const;

	virtual bool GetChildren(std::vector<SDBObject>& children) const;

	static SDBObject CreateDBObject(const QUuid& class_id, const QUuid& template_id);

protected:
	virtual void initFromDB();
	DECL_DCBASECLASS

protected:
	void setName(const QString& name);

private:
	QString name;
	QUuid id;
};

