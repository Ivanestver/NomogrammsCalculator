#include "xml.h"
#include <QXmlStreamReader>

namespace xml
{
	XmlConfiguration::XmlConfiguration()
	{
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