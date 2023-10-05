#pragma once

#include <QString>
#include <QDir>

namespace xml
{
	class XmlConfiguration
	{
	public:
		static constexpr const char* GetConfigPath();
		static const XmlConfiguration* GetInstance();


		QString GetValueByTag(const QString& tag) const;

	private:
		XmlConfiguration();
		void readData();

	private:
		std::map<QString, QString> configAttributes;
	};
}