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
	DBObject(const QUuid& id);
	DBObject(const DBObject& other);
	virtual ~DBObject() = 0;

	const QString& GetName() const;
	const QUuid& GetId() const;

	virtual bool operator==(const DBObject& other);
	virtual bool operator!=(const DBObject& other);
	virtual DBObject& operator=(const DBObject& other);

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

