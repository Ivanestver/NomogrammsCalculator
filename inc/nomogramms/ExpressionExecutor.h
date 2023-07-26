#pragma once
#include <QString>
#include "ICalculeable.h"
#include "lua/lua.hpp"
#include <LuaBridge/LuaBridge.h>

namespace nomogramms
{
	class ExpressionExecutor
	{
	public:
		ExpressionExecutor() = default;
		ExpressionExecutor(const QString& expression, const IOData* inputData);
		~ExpressionExecutor();

		void SetVariable(const QString& variableName, const SICalculeable& variable);
		bool WriteResultsTo(IOData& outputData);

		bool Exec();

		const QString& GetError() const;

	private:
		int exec(lua_State* state);
		void ret();

	private:
		void initLuaFunctions();
		void initLuaClasses();

	private:
		const IOData* inputData = nullptr;
		IOData outputData;

		QString expression;
		lua_State* L = nullptr;

		QString error;
	};
}