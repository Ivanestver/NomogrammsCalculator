#pragma once

#include <QString>
#include <QDir>

namespace xml
{
	class XmlConfigure
	{
	public:
		static const QString GetConfigPath()
		{
			return QString("%1/config.xml").arg(QDir::currentPath());
		}

		XmlConfigure() = default;
	};
}