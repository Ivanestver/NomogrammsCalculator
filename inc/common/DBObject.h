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

	QString GetName() const;
	QUuid GetId() const;

	virtual bool operator==(const DBObject& other);
	virtual bool operator!=(const DBObject& other);
	virtual DBObject& operator=(const DBObject& other);

	static SDBObject CreateDBObject(const QUuid& class_id, const QUuid& template_id);

private:
	void initialize();

protected:
	virtual void initFromDB() = 0;
	DECL_DCBASECLASS

private:
	QString name;
	QUuid id;
};

