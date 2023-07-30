#include "xml.h"
#include <QXmlStreamReader>

namespace xml
{
	XmlConfiguration::XmlConfiguration()
	{
		readData();
	}

	const QString XmlConfiguration::GetConfigPath()
	{
		return QString("%1/config.xml").arg(QDir::currentPath());
	}

	const XmlConfiguration* XmlConfiguration::GetInstance()
	{
		static XmlConfiguration instance;
		return &instance;
	}

	const QString& XmlConfiguration::GetValueByTag(const QString& tag) const
	{
		const auto it = configAttributes.find(tag);
		if (it == configAttributes.end())
			return "";

		return it->second;
	}

	void XmlConfiguration::readData()
	{
		QXmlStreamReader xmlReader;
		QFile file(GetConfigPath());
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly))
			return;

		xmlReader.setDevice(&file);

		while (!xmlReader.atEnd())
		{
			if (xmlReader.isStartElement())
			{
				configAttributes.insert({ xmlReader.name().toString(), xmlReader.readElementText() });
			}
		}
	}
}