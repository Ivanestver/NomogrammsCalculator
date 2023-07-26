#pragma once
#include <QString>
#include "ICalculeable.h"
#include <map>
#include <vector>
#include <memory>
#include "common/macros.h"

using namespace nomogramms;

struct RuleParserOutput
{
	std::vector<QString> usingSectionValues;
	std::vector<QString> inputSectionValues;
	std::vector<QString> outputSectionValues;

	void Clear();

	bool operator==(const RuleParserOutput& other) const;
	RuleParserOutput& operator=(const RuleParserOutput& other);
};

// (a,b) -> t - Принимает в расчёт номограммы (графики) с псевдонимами a,b. Выдаёт число t в качестве результата.
// [t](c,d) -> m - Принимает в расчёт номограммы (графики) с псевдонимами c,d, учитывая число t. Выдаёт число m в качестве результата.

DECL_SHARED(RuleParser);
class RuleParser
{
public:
	static RuleParserOutput Parse(const QString& rule);

private:
	RuleParser(const QString& rule);

	void processUsingAndInputValues(const QString& Part);
	void processValues(const QString& Part, const QString& openingParenthesis, const QString& closingParenthesis, std::vector<QString>& values);

private:
	RuleParserOutput ruleParserOutput;
};