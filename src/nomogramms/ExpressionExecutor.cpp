#include "nomogramms/ExpressionExecutor.h"
#include <QDebug>

namespace nomogramms
{
	ExpressionExecutor::ExpressionExecutor(const QString& expression, const IOData* inputData)
		: inputData(inputData)
		, expression(expression)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
		initLuaFunctions();
		initLuaClasses();
	}

	ExpressionExecutor::~ExpressionExecutor()
	{
		if (!L)
			lua_close(L);
	}

	bool ExpressionExecutor::SetVariable(const QString& variableName, const SICalculeable& variable, QString& error_)
	{
		try
		{
			luabridge::setGlobal(L, variable, variableName.toLocal8Bit().constData());
		}
		catch (std::logic_error& err)
		{
			error_ = QString::fromLocal8Bit("В классе 'ExpressionExecutor::SetVariable' возникла следующая ошибка: %1 \n").arg(err.what());
			return false;
		}

		return true;
	}

	bool ExpressionExecutor::WriteResultsTo(IOData& outputData_)
	{
		outputData_ += this->outputData;

		return true;
	}

	bool ExpressionExecutor::Exec()
	{
		if (!luaL_dostring(L, expression.toLocal8Bit().constData()))
		{
			error = lua_tostring(L, -1);
			return false;
		}

		return true;
	}

	const QString& ExpressionExecutor::GetError() const
	{
		return error;
	}

	int ExpressionExecutor::exec(lua_State* state)
	{
		Q_UNUSED(state);
		int args = lua_gettop(L);
		if (args < 2)
		{
			lua_pushstring(L, "The number of input values less than 1. Check it again");
			lua_error(L);
			return 0;
		}

		error.clear();
		IOData input(*inputData), output;
		for (int i = 2; i <= args; ++i)
		{
			output.Clear();
			if (lua_isuserdata(L, i))
			{
				const auto calculeable = luabridge::get<SICalculeable>(L, i);
				if (!calculeable)
					continue;

				if (!calculeable->Calculate(input, output, error))
				{
					lua_pushstring(L, error.toLocal8Bit().constData());
					lua_error(L);
					continue;
				}
			}
			
			input += output;
		}

		outputData += output;

		return 1;
	}

	void ExpressionExecutor::ret()
	{
	}

	void ExpressionExecutor::initLuaFunctions()
	{
		luabridge::getGlobalNamespace(L)
			.beginClass<ExpressionExecutor>("ExpressionExecutor")
				.addConstructor<void(*)(void)>()
				.addFunction("exec", &ExpressionExecutor::exec)
				.addFunction("ret", &ExpressionExecutor::ret)
			.endClass()
			.beginClass<ICalculeable>("ICalculeable")
			.endClass();

		luabridge::setGlobal(L, this, "c");
	}

	void ExpressionExecutor::initLuaClasses()
	{
		luabridge::getGlobalNamespace(L)
			.beginClass<SICalculeable>("SICalculeable")
			.endClass()
			.beginClass<ICalculeable>("ICalculeable")
			.endClass();
	}
}

