#include "exceptions/db_exception.h"

namespace exceptions
{
	BadRequestException::BadRequestException(const QString& message_)
		: message(message_)
	{}

	const QString& BadRequestException::GetMessage() const
	{
		return message;
	}
}