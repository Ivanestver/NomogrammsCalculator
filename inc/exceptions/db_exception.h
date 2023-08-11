#pragma once
#include <QString>

namespace exceptions
{
	class BadRequestException
	{
	public:
		BadRequestException(const QString& message_);

		const QString& GetMessage() const;
	private:
		QString message;
	};
}