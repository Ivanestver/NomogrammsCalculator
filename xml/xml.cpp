#include "xml.h"
#include <QXmlStreamReader>

namespace xml
{
	XmlConfiguration::XmlConfiguration()
	{
		readData();
	}

	constexpr const char* XmlConfiguration::GetConfigPath()
	{
		return "../xml/config.xml";
	}

	const XmlConfiguration* XmlConfiguration::GetInstance()
	{
		static XmlConfiguration instance;
		return &instance;
	}

	QString XmlConfiguration::GetValueByTag(const QString& tag) const
	{
		const auto it = configAttributes.find(tag);
		if (it == configAttributes.end())
			return "";

		return it->second;
	}

	void XmlConfiguration::readData()
	{
		QFile file(GetConfigPath());
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			return;

		QXmlStreamReader xmlReader(&file);

		while (!xmlReader.atEnd())
		{
			auto token = xmlReader.readNext();
			if (token == QXmlStreamReader::StartDocument)
				continue;

			if (token == QXmlStreamReader::StartElement)
			{
				if (xmlReader.name() == "db")
					continue;

				configAttributes.insert({ xmlReader.name().toString(), xmlReader.readElementText() });
				continue;
			}

			if (token == QXmlStreamReader::Characters)
			{
				auto text = xmlReader.text();
				text = text;
				continue;
			}
		}
	}
}