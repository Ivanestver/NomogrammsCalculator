#include "nomogramms/RuleParser.h"

RuleParserOutput RuleParser::Parse(const QString& rule)
{
	RuleParser parser(rule);
	return parser.ruleParserOutput;
}

RuleParser::RuleParser(const QString& rule)
{
	QString processedRule = rule;
	processedRule = processedRule.replace(QString(" "), QString(""));

	auto parts = processedRule.split("->");
	if (parts.isEmpty() || parts.size() != 2)
		return;

	processUsingAndInputValues(parts[0]);
	processValues("(" + parts[1] + ")", "(", ")", ruleParserOutput.outputSectionValues);
}

void RuleParser::processUsingAndInputValues(const QString& Part)
{
	auto splitted = Part.split("(");
	if (splitted.size() != 2)
		return;

	processValues(splitted[0], "[", "]", ruleParserOutput.usingSectionValues);
	processValues("(" + splitted[1], "(", ")", ruleParserOutput.inputSectionValues);
}

void RuleParser::processValues(const QString& Part, const QString& openingParenthesis, const QString& closingParenthesis, std::vector<QString>& values)
{
	if (Part.isEmpty())
		return;

	QString str = Part;
	str = str.replace(" ", "");
	{
		int indexOfOpeningParenthesis = str.indexOf(openingParenthesis);
		if (indexOfOpeningParenthesis == -1)
			return;

		int indexOfClosingParenthesis = str.indexOf(closingParenthesis);
		if (indexOfClosingParenthesis == -1)
			return;

		if (indexOfClosingParenthesis - indexOfOpeningParenthesis <= 1)
			return;
	}

	for (int i = 1; i < str.length() - 2; i++)
	{
		if (str[i] == QChar(','))
			continue;

		values.push_back(QString(str[i]));
	}
}

void RuleParserOutput::Clear()
{
	usingSectionValues.clear();
	inputSectionValues.clear();
	outputSectionValues.clear();
}

bool RuleParserOutput::operator==(const RuleParserOutput& other) const
{
	return this->usingSectionValues == other.usingSectionValues
		&& this->inputSectionValues == other.inputSectionValues
		&& this->outputSectionValues == other.outputSectionValues;
}

RuleParserOutput& RuleParserOutput::operator=(const RuleParserOutput& other)
{
	if (*this == other)
		return *this;

	this->usingSectionValues = other.usingSectionValues;
	this->inputSectionValues = other.inputSectionValues;
	this->outputSectionValues = other.outputSectionValues;

	return *this;
}
