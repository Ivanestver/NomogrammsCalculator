#pragma once
#include <memory>
#include <QUuid>


#define DECL_SHARED(__class__) \
	class __class__; \
	using S##__class__ = std::shared_ptr<__class__>

#define DECL_UNIQUE(__class__) \
	class __class__; \
	using U##__class__ = std::unique_ptr<__class__>

#define DECL_DCBASECLASS \
public: \
	virtual const QUuid GetClassID() const = 0;

#define DECL_DBCLASS(__class__) \
public: \
	static const QUuid GetCID() { return __class__##Class_ID; } \
	const QUuid GetClassID() const override { return __class__::GetCID(); } \
	\
	~__class__() override = default;\
	__class__(const __class__&) = default;\
	__class__(__class__&&) noexcept = default;\
	__class__& operator=(const __class__&) = default;\
	__class__& operator=(__class__&&) noexcept = default;
	