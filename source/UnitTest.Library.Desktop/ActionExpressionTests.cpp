#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Attributed.h"
#include "Entity.h"
#include "GameTime.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "WorldState.h"
#include "ActionExpression.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{

	template<>
	wstring ToString<ActionExpression>(const ActionExpression& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionExpressionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestConvertExpression)
		{
			ActionExpression expression;
			std::string postfix1 = expression.ConvertInfixToPostfix("4^3/(5*0)+9");
			Assert::AreEqual(postfix1,"43^50*/9+"s);

			std::string postfix2 = expression.ConvertInfixToPostfix("2+9*4/(7^3)");
			Assert::AreEqual(postfix2, "294*73^/+"s);

			Assert::ExpectException<exception>([&expression] { expression.ConvertInfixToPostfix("2+9*4/7^3)"); });
			Assert::ExpectException<exception>([&expression] { expression.ConvertInfixToPostfix("2)"); });
		}

		TEST_METHOD(TestEvaluateExpression)
		{
			ActionExpression expression;
			std::string postfix1 = expression.ConvertInfixToPostfix("2^3/(4-2)+9");
			int value1 = expression.Evaluate(postfix1);
			Assert::AreEqual(value1, 13);

			std::string postfix2 = expression.ConvertInfixToPostfix("(2*2)/4*(8/4)*5^3");
			int value2 = expression.Evaluate(postfix2);
			Assert::AreEqual(value2, 250);

			std::string postfix3 = expression.ConvertInfixToPostfix("2%2");
			int value3 = expression.Evaluate(postfix3);
			Assert::AreEqual(value3, 0);
		}

		TEST_METHOD(TestActionExpression)
		{
			ActionExpression expression;
			Datum& infix = expression.AppendAuxiliaryAttribute("MyInfixExpression");
			infix = "2^3/(4-2)+9"s;
			expression.SetInfixExpressionName("MyInfixExpression");
			Assert::AreEqual("MyInfixExpression"s, expression.InfixExpressionName());
			Datum& value = expression.AppendAuxiliaryAttribute("ExpressionValue");
			value = 0;
			expression.SetTarget("ExpressionValue"s);
			Assert::AreEqual("ExpressionValue"s, expression.Target());

			GameTime gameTime;
			WorldState worldState(gameTime);
			expression.Update(worldState);
			Assert::AreEqual(13, value.GetInteger());


			// Target does not exist
			expression.SetTarget("B");
			Assert::ExpectException<exception>([&expression, &worldState] { expression.Update(worldState); });

			// Infix expression does not exist
			expression.SetInfixExpressionName("Dummy");
			Assert::ExpectException<exception>([&expression, &worldState] { expression.Update(worldState); });

			// Target exists but cannot be incremented
			Datum& b = expression.AppendAuxiliaryAttribute("B"s);
			b = "This cannot be incremented"s;
			Assert::ExpectException<exception>([&expression, &worldState] { expression.Update(worldState); });
		}

		TEST_METHOD(ActionExpressionJsonTest)
		{
			WorldState worldState;
			ActionExpressionFactory actionExpressionFactory;
			Entity world;

			{
				// Deserialization 
				JsonTableParseHelper::SharedData sharedData(world);
				JsonParseCoordinator parseMaster(sharedData);
				JsonTableParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				const std::string filename = R"(Content\TestActionExpression.json)"s;
				parseMaster.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Action Expression"s, world.Name());

			std::string target = "MyExpressionValue"s;
			const std::string infixExpressionName = "MyInfixExpression"s;
			auto targetDatum = world.Find(target);
			auto infixExpressionDatum = world.Find(infixExpressionName);
			Assert::AreEqual(targetDatum->GetInteger(), 0);
			Assert::AreEqual(infixExpressionDatum->GetString(), "2^3/(4-2)+9"s);

			auto& actionsDatum = world.Actions();
			Assert::AreEqual(1_z, actionsDatum.Size());

			Action* action = actionsDatum[0].As<Action>();
			Assert::IsNotNull(action);
			std::string name = "EvaluateInfixExpression"s;
			Assert::AreEqual(name, action->Name());

			world.Update(worldState);
			Assert::AreEqual(targetDatum->GetInteger(), 13);
			Assert::AreEqual(infixExpressionDatum->GetString(), "2^3/(4-2)+9"s);
		}

		TEST_METHOD(Clone)
		{
			{
				ActionExpression action;

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionExpressionTests::sStartMemState;
}