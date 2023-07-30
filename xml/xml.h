#pragma once

#include <QString>
#include <QDir>

namespace xml
{
	class XmlConfiguration
	{
	public:
		static const QString GetConfigPath();
		static const XmlConfiguration* GetInstance();


		const QString& GetValueByTag(const QString& tag) const;

	private:
		XmlConfiguration();
		void readData();

	private:
		std::map<QString, QString> configAttributes;
	};
}