#pragma once
#include <memory>
#include <QUuid>

#define DECL_SHARED(__class__) \
	class __class__; \
	using S##__class__ = std::shared_ptr<__class__>

#define DECL_UNIQUE(__class__) \
	class __class__; \
	using U##__class__ = std::shared_ptr<__class__>

#define DECL_DCBASECLASS \
protected: \
	virtual const QUuid GetClassID() const = 0;

#define DECL_DBCLASS(__class__) \
public: \
	static const QUuid GetCID() { return __class__##Class_ID; } \
	const QUuid GetClassID() const override { return __class__::GetCID(); }