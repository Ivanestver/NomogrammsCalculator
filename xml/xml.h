#pragma once

#include <QString>
#include <QDir>

namespace xml
{
	class XmlConfiguration
	{
	public:
		static const QString GetConfigPath()
		{
			return QString("%1/config.xml").arg(QDir::currentPath());
		}

		XmlConfiguration();

		const QString& GetValueByTag(const QString& tag) const;

	private:
		void readData();

	private:
		std::map<QString, QString> configAttributes;
	};
}